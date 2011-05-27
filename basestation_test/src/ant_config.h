#ifndef ANT_CONFIG_H
#define ANT_CONFIG_H

#include <stdint.h>
#include <stddef.h>

#include "ant_core.h"


#define ANT_CONFIG_STATE_UNDEFINED           0
#define ANT_CONFIG_STATE_INITIAL             1
#define ANT_CONFIG_STATE_ASSIGNED            2
#define ANT_CONFIG_STATE_ID_SET              3
#define ANT_CONFIG_STATE_RF_FREQ_SET         4
#define ANT_CONFIG_STATE_PERIOD_SET          5
#define ANT_CONFIG_STATE_TX_POWER_SET        6
#define ANT_CONFIG_STATE_TIMEOUT_SET         7
#define ANT_CONFIG_STATE_LOW_TIMEOUT_SET     8
#define ANT_CONFIG_STATE_PROXIMITY_SET       9
#define ANT_CONFIG_STATE_FREQ_AGILITY_SET    10
#define ANT_CONFIG_STATE_FINAL               20

#define ANT_CONFIG_STATE_ID_UPDATE              33
#define ANT_CONFIG_STATE_RF_FREQ_UPDATE         34
#define ANT_CONFIG_STATE_PERIOD_UPDATE          35
#define ANT_CONFIG_STATE_TX_POWER_UPDATE        36
#define ANT_CONFIG_STATE_TIMEOUT_UPDATE         37
#define ANT_CONFIG_STATE_LOW_TIMEOUT_UPDATE     38
#define ANT_CONFIG_STATE_PROXIMITY_UPDATE       39
#define ANT_CONFIG_STATE_FREQ_AGILITY_UPDATE    40

#define ANT_CONFIG_STATE_ENABLE_EXT_MSG         41

#define ANT_CONFIG_STATE_OPEN_CHANNEL           50
#define ANT_CONFIG_STATE_CLOSE_CHANNEL          51


#define ANT_CONFIG_EVENT_OK                      0
#define ANT_CONFIG_EVENT_OPEN                    1
#define ANT_CONFIG_EVENT_CLOSE                   2
#define ANT_CONFIG_EVENT_FAILED                  3
#define ANT_CONFIG_EVENT_TIMEOUT                 4
#define ANT_CONFIG_EVENT_INVALID_STATE           5


#ifdef __cplusplus
extern "C" { 
#endif


//ant channel struct
struct ant_config {
  uint8_t   master:1, shared:1, bg_scan:1, freq_agility:1;
  uint8_t   tx_type;
  uint8_t   pairing:1, device_type:7;
  uint16_t  device_no; 
  uint8_t   frequency;
  uint8_t   tx_power;
  uint16_t  period;
  uint8_t   timeout_hp;
  uint8_t   timeout_lp;
  uint8_t   freq[3];
  uint8_t   proximity;
};

void ant_config_set_event_listener(void (*callback)(uint8_t, uint8_t, uint8_t));
void ant_config_handler(uint8_t channel, struct ant_buf *rx_buf, struct ant_config *config);
//static void ant_config_state(uint8_t channel, uint8_t msg_id, uint8_t event_code, struct ant_config *config);

void ant_config_channel(uint8_t channel, struct ant_config *config);
void ant_open_channel(uint8_t channel);
void ant_close_channel(uint8_t channel);

void ant_set_channel_id(uint8_t channel, struct ant_config *config);
void ant_set_channel_rf_freq(uint8_t channel, struct ant_config *config);
void ant_set_channel_period(uint8_t channel, struct ant_config *config);
void ant_set_channel_tx_power(uint8_t channel, struct ant_config *config);
void ant_set_channel_search_timeout(uint8_t channel, struct ant_config *config);
void ant_set_channel_low_prio_search_timeout(uint8_t channel, struct ant_config *config);
void ant_set_channel_priximity_search(uint8_t channel, struct ant_config *config);
void ant_config_frequency_agility(uint8_t channel, struct ant_config *config);

void ant_enable_extended_messages(uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif /* ANT_CONFIG_H */
