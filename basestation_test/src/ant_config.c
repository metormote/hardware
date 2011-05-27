#include  "ant_config.h"


static uint8_t config_state[8];

//callback for config events
static void (*config_callback)(uint8_t channel, uint8_t event, uint8_t event_code);

void ant_config_set_event_listener(void (*callback)(uint8_t, uint8_t, uint8_t event_code)) {
  config_callback=callback;
}



static void ant_config_state(uint8_t channel, uint8_t msg_id, uint8_t event_code, struct ant_config *config) {
  
  switch(config_state[channel]) {
    //config shannel state machine
    case ANT_CONFIG_STATE_INITIAL:
      if(msg_id==ANT_CONF_ASSIGN_CHANNEL && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_ASSIGNED;
        ant_core_set_channel_id(channel, config->device_no, config->pairing, config->device_type, config->tx_type);
        break;
      }
    case ANT_CONFIG_STATE_ASSIGNED:
      if(msg_id==ANT_CONF_CHANNEL_ID && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_ID_SET;
        ant_core_set_channel_rf_frequency(channel, config->frequency);
        break;
      }
    case ANT_CONFIG_STATE_ID_SET:
      if(msg_id==ANT_CONF_CHANNEL_RF_FREQUENCY && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_RF_FREQ_SET;
        ant_core_set_channel_period(channel, config->period);
        break;
      }
    case ANT_CONFIG_STATE_RF_FREQ_SET:
      if(msg_id==ANT_CONF_CHANNEL_PERIOD && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_PERIOD_SET;
        ant_core_set_channel_tx_power(channel, config->tx_power);
        break;
      }
    case ANT_CONFIG_STATE_PERIOD_SET:
      if(msg_id==ANT_CONF_CHANNEL_TX_POWER && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_TX_POWER_SET;
        ant_core_set_channel_search_timeout(channel, config->timeout_hp);
        break;
      }
    case ANT_CONFIG_STATE_TX_POWER_SET:
      if(msg_id==ANT_CONF_SEARCH_TIMEOUT && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_TIMEOUT_SET;
        ant_core_set_channel_low_prio_search_timeout(channel, config->timeout_lp);
        break;
      }
    case ANT_CONFIG_STATE_TIMEOUT_SET:
      if(msg_id==ANT_CONF_LOW_PRIO_SEARCH_TIMEOUT && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_LOW_TIMEOUT_SET;
        ant_core_set_proximity_search(channel, config->proximity);
        break;
      }
    case ANT_CONFIG_STATE_LOW_TIMEOUT_SET:
      if(msg_id==ANT_CONF_PROXIMITY_SEARCH && event_code==ANT_RESPONSE_NO_ERROR) {
        if(config->freq_agility) {
          config_state[channel]=ANT_CONFIG_STATE_PROXIMITY_SET;
          ant_core_config_frequency_agility(channel, config->freq[0], config->freq[1], config->freq[2]);
          break;
        }
        else {
          config_state[channel]=ANT_CONFIG_STATE_FINAL;
          if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
          break;
        }
      }
    case ANT_CONFIG_STATE_PROXIMITY_SET:
      if(msg_id==ANT_CONF_FREQUENCY_AGILITY && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    //update channel state machines
    case ANT_CONFIG_STATE_ID_UPDATE:
      if(msg_id==ANT_CONF_CHANNEL_ID && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_RF_FREQ_UPDATE:
      if(msg_id==ANT_CONF_CHANNEL_RF_FREQUENCY && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_PERIOD_UPDATE:
      if(msg_id==ANT_CONF_CHANNEL_PERIOD && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_TX_POWER_UPDATE:
      if(msg_id==ANT_CONF_CHANNEL_TX_POWER && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_TIMEOUT_UPDATE:
      if(msg_id==ANT_CONF_SEARCH_TIMEOUT && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_LOW_TIMEOUT_UPDATE:
      if(msg_id==ANT_CONF_LOW_PRIO_SEARCH_TIMEOUT && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_PROXIMITY_UPDATE:
      if(msg_id==ANT_CONF_PROXIMITY_SEARCH && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_FREQ_AGILITY_UPDATE:
      if(msg_id==ANT_CONF_FREQUENCY_AGILITY && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_ENABLE_EXT_MSG:
      if(msg_id==ANT_CONF_ENABLE_EXT_RX_MSG && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(ANT_CHANNEL_UNDEFINED, ANT_CONFIG_EVENT_OK, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_OPEN_CHANNEL:
      if(msg_id==ANT_CTRL_OPEN_CHANNEL && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_OPEN, ANT_RESPONSE_NO_ERROR);
        break;
      }
    case ANT_CONFIG_STATE_CLOSE_CHANNEL:
      if(msg_id==ANT_CTRL_CLOSE_CHANNEL && event_code==ANT_RESPONSE_NO_ERROR) {
        config_state[channel]=ANT_CONFIG_STATE_FINAL;
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_CLOSE, ANT_RESPONSE_NO_ERROR);
        break;
      }
    default:
      if(event_code==ANT_RESPONSE_NO_ERROR) {
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_INVALID_STATE, ANT_RESPONSE_NO_ERROR);
      }
      else {
        if(config_callback!=NULL) config_callback(channel, ANT_CONFIG_EVENT_FAILED, event_code);
      }
      break;
  }
  
}


void ant_config_handler(uint8_t channel, struct ant_buf *rx_buf, struct ant_config *config) {
  uint8_t resp_msg_id, event_code;
  
  switch(rx_buf->msg_id) {
      case ANT_EVENT_CHANNEL_RESPONSE:
        resp_msg_id=rx_buf->data[1];
        event_code=rx_buf->data[2];
        if(resp_msg_id!=1) {
          ant_config_state(channel, resp_msg_id, event_code, config);
          break;
        }
      default:
        break;
    }
}

void ant_config_channel(uint8_t channel, struct ant_config *config) {
  uint8_t type, ext;
  
  config_state[channel]=ANT_CONFIG_STATE_INITIAL;
  
  type=0;
  if(config->master) {
    if(config->shared) {
      type=ANT_SHARED_MASTER;
    }
    else {
      type=ANT_MASTER;
    }
  }
  else {
    if(config->shared) {
      type=ANT_SHARED_SLAVE;
    }
    else {
      type=ANT_SLAVE;
    }
  }
  
  ext=0;
  if(config->bg_scan) {
    ext |= ANT_ENABLE_BACKGROUND_SCANNING;
  }
  
  if(config->freq_agility) {
    ext |= ANT_ENABLE_FREQUENCY_AGILITY;
  }
  
  ant_core_assign_channel(channel, type, ANT_PUBLIC_NETWORK, ext);
}


void ant_set_channel_id(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_ID_UPDATE;
  ant_core_set_channel_id(channel, config->device_no, config->pairing, config->device_type, config->tx_type);
}

void ant_set_channel_rf_freq(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_RF_FREQ_UPDATE;
  ant_core_set_channel_rf_frequency(channel, config->frequency);
}

void ant_set_channel_period(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_PERIOD_UPDATE;
  ant_core_set_channel_period(channel, config->period);
}

void ant_set_channel_tx_power(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_TX_POWER_UPDATE;
  ant_core_set_channel_tx_power(channel, config->tx_power);
}

void ant_set_channel_search_timeout(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_TIMEOUT_UPDATE;
  ant_core_set_channel_search_timeout(channel, config->timeout_hp);
}

void ant_set_channel_low_prio_search_timeout(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_LOW_TIMEOUT_UPDATE;
  ant_core_set_channel_low_prio_search_timeout(channel, config->timeout_lp);
}

void ant_set_channel_priximity_search(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_PROXIMITY_UPDATE;
  ant_core_set_proximity_search(channel, config->proximity);
}

void ant_config_frequency_agility(uint8_t channel, struct ant_config *config) {
  config_state[channel]=ANT_CONFIG_STATE_FREQ_AGILITY_UPDATE;
  ant_core_config_frequency_agility(channel, config->freq[0], config->freq[1], config->freq[2]);
}


void ant_open_channel(uint8_t channel) {
  config_state[channel]=ANT_CONFIG_STATE_OPEN_CHANNEL;
  ant_core_open_channel(channel);
}

void ant_close_channel(uint8_t channel) {
  config_state[channel]=ANT_CONFIG_STATE_CLOSE_CHANNEL;
  ant_core_close_channel(channel);
}


void ant_enable_extended_messages(uint8_t enable) {
  config_state[0]=ANT_CONFIG_STATE_ENABLE_EXT_MSG;
  ant_core_enable_extended_messages(enable);
}




