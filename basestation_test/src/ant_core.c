#include "ant_core.h"

struct ant_queue ant_rx_queue;
struct ant_queue ant_tx_queue;

static volatile uint8_t rx_pos=0;
static volatile uint8_t rx_checksum=0;
static struct ant_buf *current_rx_buf;

static volatile uint8_t tx_pos=0;
static volatile uint8_t tx_checksum=0;
static struct ant_buf *current_tx_buf;


//forward declarations
static void ant_rx_interrupt_handler(void);
static void ant_tx_interrupt_handler(void);
static void ant_dre_interrupt_handler(void);

static struct ant_buf *allocate_buf(uint8_t data_len);
static void enqueue(struct ant_queue *queue, struct ant_buf *msg);
static struct ant_buf *dequeue(struct ant_queue *queue);



//interrupt handler for ant receive complete
ISR(USARTD0_RXC_vect)
{
    ant_rx_interrupt_handler();
}

//interrupt handler for ant data register empty
ISR(USARTD0_DRE_vect)
{
    ant_dre_interrupt_handler();
}

//interrupt handler for ant transmit complete
ISR(USARTD0_TXC_vect)
{
    ant_tx_interrupt_handler();
}


void ant_core_init() {
  ant_rx_queue.first = NULL;
  ant_rx_queue.last = NULL;
  ant_rx_queue.qlen=0;
  
  ant_tx_queue.first = NULL;
  ant_tx_queue.last = NULL;
  ant_tx_queue.qlen=0;
}



static inline void ant_tx_interrupt_handler(void) {
  
  if(current_tx_buf) {
    free(current_tx_buf->data);
    free(current_tx_buf);
  }
  
  tx_pos=0;
  current_tx_buf=dequeue(&ant_tx_queue);
  if(current_tx_buf) 
  {
    ant_dre_interrupt_handler();
  }
  
}


static inline void ant_dre_interrupt_handler(void) {
  uint8_t tx_byte;
  if(tx_pos==0) {
    tx_byte=ANT_SYNC_BYTE;
    tx_checksum=ANT_SYNC_BYTE;
  }
  else if(tx_pos==1) {
    tx_byte=current_tx_buf->data_len;
    tx_checksum^=tx_byte;
  }
  else if(tx_pos==2) {
    tx_byte=current_tx_buf->msg_id;
    tx_checksum^=tx_byte;
  }
  else if(tx_pos<(3+current_tx_buf->data_len)) {
    tx_byte=current_tx_buf->data[tx_pos-3];
    tx_checksum^=tx_byte;
  }
  else if(tx_pos==(3+current_tx_buf->data_len)) {
    tx_byte=tx_checksum;
  }
  else if(tx_pos==(4+current_tx_buf->data_len)) {
    tx_byte=0;
  }
  else {
    return;
  }
  
  // wait for rts to be deasserted...
	while (gpio_pin_is_high(nRF24AP2_RTS));
		
  usart_putchar(&USARTD0, tx_byte);
  tx_pos++;
  return;
}


static inline void ant_rx_interrupt_handler() {
  uint8_t rx_byte;
  
  rx_byte = usart_getchar(&USARTD0);
  
  //if we are at the start of a new frame we wait for a sync byte
  if(rx_pos==0 && rx_byte!=ANT_SYNC_BYTE) {
    return;
  }
    
  if(rx_pos==0) {
    //sync byte
    rx_checksum=ANT_SYNC_BYTE;
    rx_pos++;
  }
  else if(rx_pos==1) {
    //length of data byte
    current_rx_buf=(struct ant_buf*)malloc(sizeof(struct ant_buf));
    if(current_rx_buf==NULL) {
      //could not allocate memory
      rx_pos=0;
      return;
    }
    current_rx_buf->data=(uint8_t*)calloc(rx_byte, sizeof(uint8_t));
    if(current_rx_buf->data==NULL) {
      //could not allocate memory
      free(current_rx_buf);
      rx_pos=0;
      return;
    }
    current_rx_buf->next=NULL;
    current_rx_buf->data_len=rx_byte;
    rx_checksum^=rx_byte;
    rx_pos++;
  }
  else if(rx_pos==2) {
    //message id
    current_rx_buf->msg_id=rx_byte;
    rx_checksum^=rx_byte;
    rx_pos++;
  }
  else if(rx_pos<(3+current_rx_buf->data_len)) {
    //data byte
    current_rx_buf->data[rx_pos-3]=rx_byte;
    rx_checksum ^= rx_byte;
    rx_pos++;
  }
  else if(rx_pos==(3+current_rx_buf->data_len)) {
    //check checksum
    if(rx_checksum!=rx_byte) {
      //invalid checksum
      current_rx_buf->status=ANT_CHECKSUM_ERROR;
    }
    else {
      //frame complete ok
      current_rx_buf->status=ANT_OK;
    }
    
    enqueue(&ant_rx_queue, current_rx_buf);
    rx_pos=0;
  }
  else {
    //unknown state
    free(current_rx_buf->data);
    free(current_rx_buf);
    rx_pos=0;
  }
  
  return;
}




uint8_t ant_core_assign_channel(uint8_t channel, uint8_t channel_type, uint8_t network, uint8_t extended) {
  struct ant_buf *msg;
  
  msg=allocate_buf(4);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_ASSIGN_CHANNEL;
  msg->data[0]=channel;
  msg->data[1]=channel_type;
  msg->data[2]=network;
  msg->data[3]=extended;
  
  enqueue(&ant_tx_queue, msg);
  return 1;
}


uint8_t ant_core_unassign_channel(uint8_t channel) {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_UNASSIGN_CHANNEL;
  msg->data[0]=channel;
  
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_channel_id(uint8_t channel, uint16_t device_no, uint8_t pairing, uint8_t device_type, uint8_t tx_type) {
  struct ant_buf *msg;
  
  msg=allocate_buf(5);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_CHANNEL_ID;
  msg->data[0]=channel;
  msg->data[1]=(uint8_t)(device_no & 0x00ff);
  msg->data[2]=(uint8_t)((device_no & 0xff00) >> 8);
  msg->data[3]=(device_type | (pairing << 7));
  msg->data[4]=tx_type;
  enqueue(&ant_tx_queue, msg);
  return 1;
}


uint8_t ant_core_set_channel_period(uint8_t channel, uint16_t period) {
  struct ant_buf *msg;
  
  msg=allocate_buf(3);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_CHANNEL_PERIOD;
  msg->data[0]=channel;
  msg->data[1]=(uint8_t)(period & 0x00ff);
  msg->data[2]=(uint8_t)((period & 0xff00) >> 8);
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_channel_search_timeout(uint8_t channel, uint8_t timeout) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_SEARCH_TIMEOUT;
  msg->data[0]=channel;
  msg->data[1]=timeout;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_channel_rf_frequency(uint8_t channel, uint8_t rf_freq) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_CHANNEL_RF_FREQUENCY;
  msg->data[0]=channel;
  msg->data[1]=rf_freq;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

//uint8_t ant_core_set_network_key(uint8_t channel, uint8_t *key);

//uint8_t ant_core_add_channel_id(uint16_t device_no, uint8_t device_type, uint8_t tx_type, uint8_t index)

//uint8_t ant_core_config_list_id(uint8_t channel, uint8_t list_size, uint8_t exclude);

uint8_t ant_core_set_channel_tx_power(uint8_t channel, uint8_t power) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_CHANNEL_TX_POWER;
  msg->data[0]=channel;
  msg->data[1]=power;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_channel_low_prio_search_timeout(uint8_t channel, uint8_t timeout) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_LOW_PRIO_SEARCH_TIMEOUT;
  msg->data[0]=channel;
  msg->data[1]=timeout;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_config_frequency_agility(uint8_t channel, uint8_t rf_freq1, uint8_t rf_freq2, uint8_t rf_freq3) {
  struct ant_buf *msg;
  
  msg=allocate_buf(4);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_FREQUENCY_AGILITY;
  msg->data[0]=channel;
  msg->data[1]=rf_freq1;
  msg->data[2]=rf_freq2;
  msg->data[3]=rf_freq3;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_proximity_search(uint8_t channel, uint8_t threshold) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_PROXIMITY_SEARCH;
  msg->data[0]=channel;
  msg->data[1]=threshold;
  enqueue(&ant_tx_queue, msg);
  return 1;
}


uint8_t ant_core_set_transmit_power(uint8_t power) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_TRANSMIT_POWER;
  msg->data[0]=0;
  msg->data[1]=power;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_enable_extended_messages(uint8_t enable) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_ENABLE_EXT_RX_MSG;
  msg->data[0]=0;
  msg->data[1]=enable;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_enable_led(uint8_t enable) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_ENABLE_LED;
  msg->data[0]=0;
  msg->data[1]=enable;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_enable_crystal() {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CONF_CRYSTAL_ENABLE;
  msg->data[0]=0;
  enqueue(&ant_tx_queue, msg);
  return 1;
}



uint8_t ant_core_reset_system() {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_SYSTEM_RESET;
  msg->data[0]=0;
  enqueue(&ant_tx_queue, msg);
  return 1;
}


uint8_t ant_core_open_channel(uint8_t channel) {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_OPEN_CHANNEL;
  msg->data[0]=channel;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_close_channel(uint8_t channel) {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_CLOSE_CHANNEL;
  msg->data[0]=channel;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_request_message(uint8_t channel, uint8_t message_id) {
  struct ant_buf *msg;
  
  msg=allocate_buf(2);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_REQUEST_MESSAGE;
  msg->data[0]=channel;
  msg->data[1]=message_id;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_open_rx_scan_mode() {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_OPEN_RX_SCAN_MODE;
  msg->data[0]=0;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_sleep_message() {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_CTRL_SLEEP_MESSAGE;
  msg->data[0]=0;
  enqueue(&ant_tx_queue, msg);
  return 1;
}



uint8_t ant_core_send_broadcast_data(uint8_t channel, uint8_t data_len, uint8_t *data) {
  struct ant_buf *msg;
  int i;
  
  msg=allocate_buf(9);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_DATA_BROADCAST;
  msg->data[0]=channel;
  for(i=0;i<8;i++) {
    if(i>=data_len) break;
    msg->data[i+1]=data[i];
  }
  enqueue(&ant_tx_queue, msg);
  if(!current_tx_buf) {
    ant_tx_interrupt_handler();
  }
  return 1;
}


uint8_t ant_core_send_acknowledged_data(uint8_t channel, uint8_t data_len, uint8_t *data) {
  struct ant_buf *msg;
  int i;
  
  msg=allocate_buf(9);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_DATA_ACKNOWLEDGE;
  msg->data[0]=channel;
  for(i=0;i<8;i++) {
    if(i>=data_len) break;
    msg->data[i+1]=data[i];
  }
  enqueue(&ant_tx_queue, msg);
  if(!current_tx_buf) {
    ant_tx_interrupt_handler();
  }
  return 1;
}


uint8_t ant_core_send_burst_transfer(uint8_t channel, uint8_t head_len, uint8_t *head, uint32_t data_len, uint8_t *data) {
  struct ant_buf *msg;
  int no_of_pgks, last_data_len;
  int i, j;
  
  //first packet
  msg=allocate_buf(9);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_DATA_BURST_TRANSFER;
  msg->data[0]=channel;
  for(i=0;i<8;i++) {
    if(i>=head_len) break;
    msg->data[i+1]=head[i];
  }
  enqueue(&ant_tx_queue, msg);
  
  //packages
  no_of_pgks=data_len/8;
  last_data_len=data_len % 8;
  
  for(j=0;j<no_of_pgks; j++) {
    msg=allocate_buf(9);
    if(!msg) {
      return 0;
    }
    msg->msg_id=ANT_DATA_BURST_TRANSFER;
    msg->data[0]=((j % 4) << 5) | channel;
    for(i=0;i<8;i++) {
      msg->data[i+1]=data[8*j+i];
    }
    enqueue(&ant_tx_queue, msg);
  }
  
  //last packet
  msg=allocate_buf(9);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_DATA_BURST_TRANSFER;
  msg->data[0]=0x80 | ((j % 4) << 5) | channel;
  for(i=0;i<8;i++) {
    if(i>=data_len) break;
    msg->data[i+1]=data[8*j+i];
  }
  enqueue(&ant_tx_queue, msg);
  
  if(!current_tx_buf) {
    ant_tx_interrupt_handler();
  }
  return 1;
}




uint8_t ant_core_init_cw_test_mode() {
  struct ant_buf *msg;
  
  msg=allocate_buf(1);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_TEST_CW_INIT;
  msg->data[0]=0;
  enqueue(&ant_tx_queue, msg);
  return 1;
}

uint8_t ant_core_set_cw_test_mode(uint8_t tx_power, uint8_t rf_freq) {
  struct ant_buf *msg;
  
  msg=allocate_buf(3);
  if(!msg) {
    return 0;
  }
  msg->msg_id=ANT_TEST_CW_TEST;
  msg->data[0]=0;
  msg->data[1]=tx_power;
  msg->data[2]=rf_freq;
  enqueue(&ant_tx_queue, msg);
  return 1;
}





static struct ant_buf *allocate_buf(uint8_t data_len) {
  struct ant_buf *msg;
  
  msg=(struct ant_buf*)malloc(sizeof(struct ant_buf));
  if(msg==NULL) {
    //could not allocate memory
    return NULL;
  }
  
  msg->data=(uint8_t*)calloc(data_len, sizeof(uint8_t));
  if(msg->data==NULL) {
    //could not allocate memory
    free(msg);
    return NULL;
  }
  
  msg->next=NULL;
  msg->data_len=data_len;
  
  return msg;
}

static void enqueue(struct ant_queue *queue, struct ant_buf *msg) {
  cpu_irq_disable();
  
  queue->last = msg;
  if (queue->first == NULL) {
    queue->first = msg;
  } 
  else 
  {
    queue->last->next = msg;
  }
  queue->qlen++;
  
  cpu_irq_enable();
}

static struct ant_buf *dequeue(struct ant_queue *queue) {
  struct ant_buf *msg=queue->first;
  cpu_irq_disable();
  if (msg != NULL) {
    queue->first = msg->next;
    if(queue->last == msg) {
      queue->last = NULL;
    }
    msg->next=NULL;
    queue->qlen--;
  } 
  cpu_irq_enable();
  return msg;
}
