/**
 * @file        tlv320aic3106.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320AIC3106 audio CODEC driver.
 * @version     0.1
 * @date        2019-10-02
 * 
 * @copyright   Copyright (c) 2019 Danius Kalvaitis
 * 
 */

#ifndef TLV320AIC3106_H
#define TLV320AIC3106_H

#include "dk_twi_mngr.h"
#include "sdk_errors.h"
#include "dk_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tlv320aic3106_s tlv320aic3106_t; // Forward declaration

typedef enum
{
	TLV320AIC3106_ACTIVE_PAGE_0 = 0,
	TLV320AIC3106_ACTIVE_PAGE_1 = 1
} tlv320aic3106_active_page_t;

typedef enum
{
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_1    = 0x00,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_1_5  = 0x01,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_2    = 0x02,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_2_5  = 0x03,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_3    = 0x04,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_3_5  = 0x05,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_4    = 0x06,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_4_5  = 0x07,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_5    = 0x08,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_5_5  = 0x09,
	TLV320AIC3106_ADC_SAMPLE_RATE_SELECT_FSREF_DIV_6    = 0x0A,
} tlv320aic3106_adc_sample_rate_select_t;

typedef enum
{
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_1    = 0x00,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_1_5  = 0x01,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_2    = 0x02,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_2_5  = 0x03,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_3    = 0x04,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_3_5  = 0x05,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_4    = 0x06,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_4_5  = 0x07,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_5    = 0x08,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_5_5  = 0x09,
	TLV320AIC3106_DAC_SAMPLE_RATE_SELECT_FSREF_DIV_6    = 0x0A,
} tlv320aic3106_dac_sample_rate_select_t;

typedef struct
{
	tlv320aic3106_dac_sample_rate_select_t  dac_sample_rate:4;
	tlv320aic3106_adc_sample_rate_select_t  adc_sample_rate:4;
} tlv320aic3106_sample_rate_select_t;

typedef enum
{
	TLV320AIC3106_PLL_Q_16  = 0x00,
	TLV320AIC3106_PLL_Q_17  = 0x01,
	TLV320AIC3106_PLL_Q_2   = 0x02,
	TLV320AIC3106_PLL_Q_3   = 0x03,
	TLV320AIC3106_PLL_Q_4   = 0x04,
	TLV320AIC3106_PLL_Q_5   = 0x05,
	TLV320AIC3106_PLL_Q_6   = 0x06,
	TLV320AIC3106_PLL_Q_7   = 0x07,
	TLV320AIC3106_PLL_Q_8   = 0x08,
	TLV320AIC3106_PLL_Q_9   = 0x09,
	TLV320AIC3106_PLL_Q_10  = 0x0A,
	TLV320AIC3106_PLL_Q_11  = 0x0B,
	TLV320AIC3106_PLL_Q_12  = 0x0C,
	TLV320AIC3106_PLL_Q_13  = 0x0D,
	TLV320AIC3106_PLL_Q_14  = 0x0E,
	TLV320AIC3106_PLL_Q_15  = 0x0F,
} tlv320aic3106_pll_q_t;

typedef enum
{
	TLV320AIC3106_PLL_P_8   = 0x00,
	TLV320AIC3106_PLL_P_1   = 0x01,
	TLV320AIC3106_PLL_P_2   = 0x02,
	TLV320AIC3106_PLL_P_3   = 0x03,
	TLV320AIC3106_PLL_P_4   = 0x04,
	TLV320AIC3106_PLL_P_5   = 0x05,
	TLV320AIC3106_PLL_P_6   = 0x06,
	TLV320AIC3106_PLL_P_7   = 0x07,
} tlv320aic3106_pll_p_t;

typedef struct
{
	tlv320aic3106_pll_p_t   pll_p       :3;
	tlv320aic3106_pll_q_t   pll_q       :4;
	bool                    pll_enabled :1;
} tlv320aic3106_pll_prog_reg_a_t;

typedef struct
{
	uint8_t _padding0   :2;
	uint8_t pll_j       :6;
} tlv320aic3106_pll_prog_reg_b_t;

typedef struct
{
	uint8_t _padding0   :2;
	uint8_t pll_d       :6;
} tlv320aic3106_pll_prog_reg_d_t;

typedef struct
{
	tlv320aic3106_pll_prog_reg_a_t reg_a;
	tlv320aic3106_pll_prog_reg_b_t reg_b;
	uint16_t                       val_d;
} tlv320aic3106_pll_prog_t;

typedef enum
{
	TLV320AIC3106_FSREF_SETTING_48KHZ   = 0x00,
	TLV320AIC3106_FSREF_SETTING_44_1KHZ = 0x01,
} tlv320aic3106_fsref_setting_t;

typedef enum
{
	TLV320AIC3106_LEFT_DAC_DATAPATH_CTRL_OFF        = 0x00,
	TLV320AIC3106_LEFT_DAC_DATAPATH_CTRL_LEFT_EN    = 0x01,
	TLV320AIC3106_LEFT_DAC_DATAPATH_CTRL_RIGHT_EN   = 0x02,
	TLV320AIC3106_LEFT_DAC_DATAPATH_CTRL_MONO_MIX   = 0x03,
} tlv320aic3106_left_dac_datapath_ctrl_t;

typedef enum
{
	TLV320AIC3106_RIGHT_DAC_DATAPATH_CTRL_OFF       = 0x00,
	TLV320AIC3106_RIGHT_DAC_DATAPATH_CTRL_LEFT_EN   = 0x01,
	TLV320AIC3106_RIGHT_DAC_DATAPATH_CTRL_RIGHT_EN  = 0x02,
	TLV320AIC3106_RIGHT_DAC_DATAPATH_CTRL_MONO_MIX  = 0x03,
} tlv320aic3106_right_dac_datapath_ctrl_t;

typedef struct
{
	uint8_t                                 _padding0               :1;
	tlv320aic3106_right_dac_datapath_ctrl_t right_dac_datapath_ctrl :2;
	tlv320aic3106_left_dac_datapath_ctrl_t  left_dac_datapath_ctrl  :2;
	bool                                    dac_dual_rate_enabled   :1;
	bool                                    adc_dual_rate_enabled   :1;
	tlv320aic3106_fsref_setting_t           fsref_setting           :1;
} tlv320aic3106_datapath_setup_t;

typedef enum
{
	TLV320AIC3106_DIG_MIC_SUPPORT_DISABLED  = 0x00,
	TLV320AIC3106_DIG_MIC_SUPPORT_EN_OR_128 = 0x01,
	TLV320AIC3106_DIG_MIC_SUPPORT_EN_OR_64  = 0x02,
	TLV320AIC3106_DIG_MIC_SUPPORT_EN_OR_32  = 0x03,
} tlv320aic3106_dig_mic_support_t;

typedef struct
{
	tlv320aic3106_dig_mic_support_t dig_mic_support     :2;
	bool                            dig_3d_effects_en   :1;
	uint8_t                         _padding0           :1;
	bool                            bclk_drive          :1;
	bool                            dout_idle_high_z    :1;
	bool                            wclk_dir_output     :1;
	bool                            bclk_dir_output     :1;
} tlv320aic3106_audio_ser_data_interface_ctrl_a_t;

typedef enum
{
	TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_TF_MODE_I2S          = 0x00,
	TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_TF_MODE_DSP          = 0x01,
	TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_TF_MODE_RIGHT_JUST   = 0x02,
	TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_TF_MODE_LEFT_JUST    = 0x03,
} tlv320aic3106_audio_ser_data_interface_tf_mode_t;

typedef enum
{
	TLV320AIC3106_AUDIO_SER_DATA_WORD_LENGTH_16BITS = 0x00,
	TLV320AIC3106_AUDIO_SER_DATA_WORD_LENGTH_20BITS = 0x01,
	TLV320AIC3106_AUDIO_SER_DATA_WORD_LENGTH_24BITS = 0x02,
	TLV320AIC3106_AUDIO_SER_DATA_WORD_LENGTH_32BITS = 0x03,
} tlv320aic3106_audio_ser_data_word_length_t;

typedef struct
{
	bool                                                re_sync_with_soft_mute  :1;
	bool                                                re_sync_adc             :1;
	bool                                                re_sync_dac             :1;
	bool                                                bclk_256_mode           :1;
	tlv320aic3106_audio_ser_data_word_length_t          word_length             :2;
	tlv320aic3106_audio_ser_data_interface_tf_mode_t    interface_tf_mode       :2;
} tlv320aic3106_audio_ser_data_interface_ctrl_b_t;

typedef enum
{
	TLV320AIC3106_PLL_R_16  = 0x00,
	TLV320AIC3106_PLL_R_1   = 0x01,
	TLV320AIC3106_PLL_R_2   = 0x02,
	TLV320AIC3106_PLL_R_3   = 0x03,
	TLV320AIC3106_PLL_R_4   = 0x04,
	TLV320AIC3106_PLL_R_5   = 0x05,
	TLV320AIC3106_PLL_R_6   = 0x06,
	TLV320AIC3106_PLL_R_7   = 0x07,
	TLV320AIC3106_PLL_R_8   = 0x08,
	TLV320AIC3106_PLL_R_9   = 0x09,
	TLV320AIC3106_PLL_R_10  = 0x0A,
	TLV320AIC3106_PLL_R_11  = 0x0B,
	TLV320AIC3106_PLL_R_12  = 0x0C,
	TLV320AIC3106_PLL_R_13  = 0x0D,
	TLV320AIC3106_PLL_R_14  = 0x0E,
	TLV320AIC3106_PLL_R_15  = 0x0F,
} tlv320aic3106_pll_r_t;

typedef struct
{
	tlv320aic3106_pll_r_t   pll_r               :4;
	bool                    right_dac_overflow  :1;
	bool                    left_dac_overflow   :1;
	bool                    right_adc_overflow  :1;
	bool                    left_adc_overflow   :1;
} tlv320aic3106_audio_codec_overflow_flag_t;

typedef enum
{
	TLV320AIC3106_HIGHPASS_FILTER_CTRL_DISABLED         = 0x00,
	TLV320AIC3106_HIGHPASS_FILTER_CTRL_0_0045_X_ADCFS   = 0x01,
	TLV320AIC3106_HIGHPASS_FILTER_CTRL_0_0125_X_ADCFS   = 0x02,
	TLV320AIC3106_HIGHPASS_FILTER_CTRL_0_025_X_ADCFS    = 0x03,
} tlv320aic3106_highpass_filter_ctrl_t;

typedef struct
{
	bool                                    right_dac_de_emphasis_en        :1;
	bool                                    right_dac_dig_effects_en        :1;
	bool                                    left_dac_de_emphasis_en         :1;
	bool                                    left_dac_dig_effects_en         :1;
	tlv320aic3106_highpass_filter_ctrl_t    right_adc_highpass_filter_ctrl  :2;
	tlv320aic3106_highpass_filter_ctrl_t    left_adc_highpass_filter_ctrl   :2;
} tlv320aic3106_audio_codec_digital_filter_ctrl_t;

typedef enum
{
	TLV320AIC3106_HEADSET_TYPE_DETECT_RES_NO_HEADSET        = 0x00,
	TLV320AIC3106_HEADSET_TYPE_DETECT_RES_HEADSET_NO_MIC    = 0x01,
	TLV320AIC3106_HEADSET_TYPE_DETECT_RES_HEADSET_WITH_MIC  = 0x03,
} tlv320aic3106_headset_type_detect_res_t;

typedef enum
{
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_16MS   = 0x00,
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_32MS   = 0x01,
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_64MS   = 0x02,
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_128MS  = 0x03,
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_256MS  = 0x04,
	TLV320AIC3106_HEADSET_JACK_DEBOUNCE_CTRL_512MS  = 0x05,
} tlv320aic3106_headset_jack_debounce_ctrl_t;

typedef enum
{
	TLV320AIC3106_HEADSET_BTN_DEBOUNCE_CTRL_0MS     = 0x00,
	TLV320AIC3106_HEADSET_BTN_DEBOUNCE_CTRL_8MS     = 0x01,
	TLV320AIC3106_HEADSET_BTN_DEBOUNCE_CTRL_16MS    = 0x02,
	TLV320AIC3106_HEADSET_BTN_DEBOUNCE_CTRL_32MS    = 0x03,
} tlv320aic3106_headset_btn_debounce_ctrl_t;

typedef struct
{
	tlv320aic3106_headset_btn_debounce_ctrl_t   headset_btn_debounce_ctrl   :2;
	tlv320aic3106_headset_jack_debounce_ctrl_t  headset_jack_debounce_ctrl  :3;
	tlv320aic3106_headset_type_detect_res_t     headset_detect_res          :2;
	bool                                        headset_detect_en           :1;
} tlv320aic3106_headset_btn_press_detect_a_t;

typedef struct
{
	uint8_t _padding0                           :3;
	bool    stereo_out_pseudodifferential_en    :1;
	bool    headset_detected                    :1;
	bool    button_press_detected               :1;
	bool    stereo_out_fully_differential_en    :1;
	bool    hi_pow_out_ac_coupled_drive_en      :1;
} tlv320aic3106_headset_btn_press_detect_b_t;

typedef struct
{
	uint8_t adc_pga_gain:7;
	bool    adc_pga_mute:1;
} tlv320aic3106_adc_pga_gain_ctrl_t;

typedef enum
{
	TLV320AIC3106_IN_LVL_CTRL_GAIN_0DB      = 0x00,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_1_5DB  = 0x01,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_3DB    = 0x02,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_4_5DB  = 0x03,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_6DB    = 0x04,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_7_5DB  = 0x05,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_9DB    = 0x06,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_10_5DB = 0x07,
	TLV320AIC3106_IN_LVL_CTRL_GAIN_N_12DB   = 0x08,
	TLV320AIC3106_NOT_CONNECTED             = 0x0F,
} tlv320aic3106_in_lvl_ctrl_gain_t;

typedef struct
{
	tlv320aic3106_in_lvl_ctrl_gain_t mic3r_in_lvl_ctrl_adc_pga_mix:4;
	tlv320aic3106_in_lvl_ctrl_gain_t mic3l_in_lvl_ctrl_adc_pga_mix:4;
} tlv320aic3106_mic3lr_to_adc_ctrl_t;

typedef enum
{
	TLV320AIC3106_SOFT_STEP_CTRL_1_PER_FS   = 0x00,
	TLV320AIC3106_SOFT_STEP_CTRL_1_PER_2FS  = 0x01,
	TLV320AIC3106_SOFT_STEP_CTRL_DISABLED   = 0x02,
} tlv320aic3106_soft_step_ctrl_t;

typedef struct
{
	tlv320aic3106_soft_step_ctrl_t      left_adc_pga_soft_step_ctrl     :2;
	bool                                left_adc_ch_power_up            :1;
	tlv320aic3106_in_lvl_ctrl_gain_t    line1l_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line1l_to_left_adc_ctrl_t;

typedef struct
{
	uint8_t                             _padding0                       :2;
	bool                                left_adc_weak_bias_en           :1;
	tlv320aic3106_in_lvl_ctrl_gain_t    line2l_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line2l_to_left_adc_ctrl_t;

typedef struct
{
	uint8_t                             _padding0                       :3;
	tlv320aic3106_in_lvl_ctrl_gain_t    line1r_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line1r_to_left_adc_ctrl_t;

typedef struct
{
	tlv320aic3106_soft_step_ctrl_t      left_adc_pga_soft_step_ctrl     :2;
	bool                                right_adc_ch_power_up           :1;
	tlv320aic3106_in_lvl_ctrl_gain_t    line1r_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line1r_to_right_adc_ctrl_t;

typedef struct
{
	uint8_t                             _padding0                       :2;
	bool                                right_adc_weak_bias_en          :1;
	tlv320aic3106_in_lvl_ctrl_gain_t    line2r_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line2r_to_right_adc_ctrl_t;

typedef struct
{
	uint8_t                             _padding0                       :3;
	tlv320aic3106_in_lvl_ctrl_gain_t    line1l_in_lvl_ctrl_adc_pga_mix  :4;
	bool                                fully_differential_mode_en      :1;
} tlv320aic3106_line1l_to_right_adc_ctrl_t;

typedef enum
{
	TLV320AIC3106_MICBIAS_LVL_POWER_DOWN    = 0x00,
	TLV320AIC3106_MICBIAS_LVL_2_0V          = 0x01,
	TLV320AIC3106_MICBIAS_LVL_2_5V          = 0x02,
	TLV320AIC3106_MICBIAS_LVL_AVDD          = 0x03,
} tlv320aic3106_micbias_lvl_t;

typedef enum
{
	TLV320AIC3106_DIG_MIC_CTRL_LEFT_RIGHT_DIG_MIC       = 0x00,
	TLV320AIC3106_DIG_MIC_CTRL_LEFT_DIG_MIC_RIGHT_ADC   = 0x01,
	TLV320AIC3106_DIG_MIC_CTRL_LEFT_ADC_RIGHT_DIG_MIC   = 0x02,
} tlv320aic3106_dig_mic_ctrl_t;

typedef struct
{
	uint8_t                         _padding0   :4;
	tlv320aic3106_dig_mic_ctrl_t    dig_mic_ctrl:2;
	tlv320aic3106_micbias_lvl_t	    micbias_lvl :2;
} tlv320aic3106_micbias_ctrl_t;

typedef enum
{
	TLV320AIC3106_AGC_TARGET_LVL_N_5_5DB    = 0x00,
	TLV320AIC3106_AGC_TARGET_LVL_N_8DB      = 0x01,
	TLV320AIC3106_AGC_TARGET_LVL_N_10DB     = 0x02,
	TLV320AIC3106_AGC_TARGET_LVL_N_12DB     = 0x03,
	TLV320AIC3106_AGC_TARGET_LVL_N_14DB     = 0x04,
	TLV320AIC3106_AGC_TARGET_LVL_N_17DB     = 0x05,
	TLV320AIC3106_AGC_TARGET_LVL_N_20DB     = 0x06,
	TLV320AIC3106_AGC_TARGET_LVL_N_24DB     = 0x07,
} tlv320aic3106_agc_target_lvl_t;

typedef enum
{
	TLV320AIC3106_AGC_ATTACK_TIME_8MS   = 0x00,
	TLV320AIC3106_AGC_ATTACK_TIME_11MS  = 0x01,
	TLV320AIC3106_AGC_ATTACK_TIME_16MS  = 0x02,
	TLV320AIC3106_AGC_ATTACK_TIME_20MS  = 0x03,
} tlv320aic3106_agc_attack_time_t;

typedef enum
{
	TLV320AIC3106_AGC_DECAY_TIME_100MS  = 0x00,
	TLV320AIC3106_AGC_DECAY_TIME_200MS  = 0x01,
	TLV320AIC3106_AGC_DECAY_TIME_400MS  = 0x02,
	TLV320AIC3106_AGC_DECAY_TIME_500MS  = 0x03,
} tlv320aic3106_agc_decay_time_t;

typedef struct
{
	tlv320aic3106_agc_decay_time_t  decay_time  :2;
	tlv320aic3106_agc_attack_time_t attack_time :2;
	tlv320aic3106_agc_target_lvl_t  target_lvl  :3;
	bool                            en          :1;
} tlv320aic3106_agc_ctrl_a_t;

typedef struct
{
	uint8_t _padding0       :1;
	uint8_t max_allowed_gain:7;
} tlv320aic3106_agc_ctrl_b_t;

typedef enum
{
	TLV320AIC3106_NOISE_GATE_HYST_LVL_1DB       = 0x00,
	TLV320AIC3106_NOISE_GATE_HYST_LVL_2DB       = 0x01,
	TLV320AIC3106_NOISE_GATE_HYST_LVL_3DB       = 0x02,
	TLV320AIC3106_NOISE_GATE_HYST_LVL_DISABLED  = 0x03,
} tlv320aic3106_noise_gate_hyst_lvl_t;

typedef struct
{
	bool                                clip_step_en        :1;
	uint8_t                             noise_gate_ths      :5;
	tlv320aic3106_noise_gate_hyst_lvl_t noise_gate_hyst_lvl :2;
} tlv320aic3106_agc_ctrl_c_t;

typedef enum
{
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_0MS     = 0x00,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_0_5MS   = 0x01,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_1MS     = 0x02,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_2MS     = 0x03,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_4MS     = 0x04,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_8MS     = 0x05,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_16MS    = 0x06,
	TLV320AIC3106_AGC_NOISE_GATE_SIGNAL_DETECT_DEBOUNCE_32MS    = 0x07,
} tlv320aic3106_agc_noise_gate_signal_detect_debounce_t;

typedef struct
{
	tlv320aic3106_agc_noise_gate_signal_detect_debounce_t   signal_detect_debounce  :3;
	uint8_t                                                 noise_detect_debounce   :5;
} tlv320aic3106_agc_noise_gate_debounce_t;

typedef struct
{
	bool    right_agc_saturated                         :1;
	bool    right_agc_signal_pwr_less_than_noise_ths    :1;
	bool    right_adc_powered_up                        :1;
	bool    right_adc_applied_and_programmed_gains_match:1;
	bool    left_agc_saturated                          :1;
	bool    left_agc_signal_pwr_less_than_noise_ths     :1;
	bool    left_adc_powered_up                         :1;
	bool    left_adc_applied_and_programmed_gains_match :1;
} tlv320aic3106_adc_flag_t;

typedef enum
{
	TLC320AIC3106_HPLCOM_OUT_DRV_DIFF_HPLOUT    = 0x00,
	TLC320AIC3106_HPLCOM_OUT_DRV_CONST_VCMOUT   = 0x01,
	TLC320AIC3106_HPLCOM_OUT_DRV_SNGL_ENDED     = 0x02,
} tlc320aic3106_hplcom_out_drv_t;

typedef struct
{
	uint8_t                         _padding0           :4; // Datasheet contains a mistake!
	tlc320aic3106_hplcom_out_drv_t  hplcom_out_drv_cfg  :2;
	bool                            right_dac_powered_up:1;
	bool                            left_dac_powered_up :1;
} tlv320aic3106_ac_pwr_and_out_drv_ctrl_t;

typedef enum
{
	TLC320AIC3106_HPRCOM_OUT_DRV_DIFF_HPROUT                    = 0x00,
	TLC320AIC3106_HPRCOM_OUT_DRV_CONST_VCMOUT                   = 0x01,
	TLC320AIC3106_HPRCOM_OUT_DRV_SNGL_ENDED                     = 0x02,
	TLC320AIC3106_HPRCOM_OUT_DRV_DIFF_HPLCOM                    = 0x03,
	TLC320AIC3106_HPRCOM_OUT_DRV_EXT_FBACK_HPLCOM_CONST_VCMOUT  = 0x04,
} tlc320aic3106_hprcom_out_drv_t;

typedef enum
{
	TLC320AIC3106_SHORT_CIRCUIT_PROTECTION_MODE_CURRENT_LIMIT   = 0x00,
	TLC320AIC3106_SHORT_CIRCUIT_PROTECTION_MODE_PWR_DOWN        = 0x01,
} tlc320aic3106_short_circuit_protection_mode_t;

typedef struct
{
	uint8_t                                         _padding1                       :1;
	tlc320aic3106_short_circuit_protection_mode_t   short_circuit_protection_mode   :1;
	bool                                            short_circuit_protection_en     :1;
	tlc320aic3106_hprcom_out_drv_t                  hprcom_out_drv_cfg              :3;
	uint8_t                                         _padding0                       :2;
} tlv320aic3106_hi_pwr_out_drv_ctrl_t;

typedef enum
{
	TLV320AIC3106_OUT_COMMON_MODE_VOLTAGE_1_35V = 0x00,
	TLV320AIC3106_OUT_COMMON_MODE_VOLTAGE_1_5V  = 0x01,
	TLV320AIC3106_OUT_COMMON_MODE_VOLTAGE_1_65V = 0x02,
	TLV320AIC3106_OUT_COMMON_MODE_VOLTAGE_1_8V  = 0x03,
} tlv320aic3106_out_common_mode_voltage_t;

typedef enum
{
	TLV320AIC3106_LINE2X_BYPASS_PATH_DISABLED           = 0x00,
	TLV320AIC3106_LINE2X_BYPASS_PATH_LINE2XP_SNGL_END   = 0x01,
	TLV320AIC3106_LINE2X_BYPASS_PATH_LINE2XM_SNGL_END   = 0x02,
	TLV320AIC3106_LINE2X_BYPASS_PATH_LINE2XP_M_DIFF     = 0x03,
} tlv320aic3106_line2x_bypass_path_t;

typedef struct
{
	tlv320aic3106_soft_step_ctrl_t          out_volume_ctrl_soft_step   :2;
	tlv320aic3106_line2x_bypass_path_t      line2r_bypass_path          :2;
	tlv320aic3106_line2x_bypass_path_t      line2l_bypass_path          :2;
	tlv320aic3106_out_common_mode_voltage_t out_common_mode_voltage     :2;
} tlv320aic3106_hi_pwr_out_stage_ctrl_t;

typedef enum
{
	TLV320AIC3106_XDAC_OUT_SWITCH_DAC_X1    = 0x00,
	TLV320AIC3106_XDAC_OUT_SWITCH_DAC_X3    = 0x01,
	TLV320AIC3106_XDAC_OUT_SWITCH_DAC_X2    = 0x02,
} tlv320aic3106_xdac_out_switch_t;

typedef enum
{
	TLV320AIC3106_DAC_DIG_VOL_CTRL_LEFT_AND_RIGHT_INDEPENDANT   = 0x00,
	TLV320AIC3106_DAC_DIG_VOL_CTRL_LEFT_FOLLOWS_RIGHT_CHANNEL   = 0x01,
	TLV320AIC3106_DAC_DIG_VOL_CTRL_RIGHT_FOLLOWS_LEFT_CHANNEL   = 0x02,
} tlv320aic3106_dac_dig_vol_ctrl_t;

typedef struct
{
	tlv320aic3106_dac_dig_vol_ctrl_t    dac_dig_vol_ctrl    :2;
	uint8_t                             _padding0           :2;
	tlv320aic3106_xdac_out_switch_t     right_dac_out_switch:2;
	tlv320aic3106_xdac_out_switch_t     left_dac_out_switch :2;
} tlv320aic3106_dac_out_switch_ctrl_t;

typedef enum
{
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_0US    = 0x00,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_10US   = 0x01,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_100US  = 0x02,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_1MS    = 0x03,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_10MS   = 0x04,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_50MS   = 0x05,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_100MS  = 0x06,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_200MS  = 0x07,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_400MS  = 0x08,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_800MS  = 0x09,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_2S     = 0x0A,
	TLV320AIC3106_OUT_DRV_POWER_ON_DELAY_4S     = 0x0B,
} tlv320aic3106_out_drv_power_on_delay_t;

typedef enum
{
	TLV320AIC3106_OUT_DRV_RAMP_UP_STEP_TIME_0MS = 0x00,
	TLV320AIC3106_OUT_DRV_RAMP_UP_STEP_TIME_1MS = 0x01,
	TLV320AIC3106_OUT_DRV_RAMP_UP_STEP_TIME_2MS = 0x02,
	TLV320AIC3106_OUT_DRV_RAMP_UP_STEP_TIME_4MS = 0x03,
} tlv320aic3106_drv_ramp_up_step_time_t;

typedef enum
{
	TLV320AIC3106_WEAK_OUT_COMMON_MODE_VOLTAGE_RES_DIV_OF_AVDD  = 0x00,
	TLV320AIC3106_WEAK_OUT_COMMON_MODE_VOLTAGE_BANDGAP_REF      = 0x01,
} tlv320aic3106_weak_out_common_mode_voltage_t;


typedef struct
{
	uint8_t                                         _padding0                   :1;
	tlv320aic3106_weak_out_common_mode_voltage_t    weak_out_common_mode_voltage:1;
	tlv320aic3106_drv_ramp_up_step_time_t           drv_ramp_up_step_time       :2;
	tlv320aic3106_out_drv_power_on_delay_t          out_drv_pwr_on_delay        :4;
} tlv320aic3106_out_drv_pop_reduction_t;

typedef struct
{
	uint8_t dac_dig_volume_ctrl :7;
	bool    dac_muted           :1;
} tlv320aic3106_dac_dig_volume_ctrl_t;

typedef struct
{
	uint8_t x_to_y_ana_volume_ctrl  :7;
	bool    routed_to_y             :1;
} tlv320aic3106_x_to_y_volume_ctrl_t;

typedef enum
{
	TLV320AIC3106_X_OUT_LVL_0DB = 0x00,
	TLV320AIC3106_X_OUT_LVL_1DB = 0x01,
	TLV320AIC3106_X_OUT_LVL_2DB = 0x02,
	TLV320AIC3106_X_OUT_LVL_3DB = 0x03,
	TLV320AIC3106_X_OUT_LVL_4DB = 0x04,
	TLV320AIC3106_X_OUT_LVL_5DB = 0x05,
	TLV320AIC3106_X_OUT_LVL_6DB = 0x06,
	TLV320AIC3106_X_OUT_LVL_7DB = 0x07,
	TLV320AIC3106_X_OUT_LVL_8DB = 0x08,
	TLV320AIC3106_X_OUT_LVL_9DB = 0x09,
} tlv320aic3106_x_out_lvl_t;

typedef struct
{
	bool                        power_en                        :1;
	bool                        not_all_programmed_gains_applied:1;
	bool                        high_z_power_down               :1;
	bool                        not_muted                       :1;
	tlv320aic3106_x_out_lvl_t   xout_out_lvl                    :4;
} tlv320aic3106_x_out_lvl_ctrl_t;

typedef struct
{
	uint8_t _padding0               :1;
	bool    hprout_powered_up       :1;
	bool    hplout_powered_up       :1;
	bool    right_lop_m_powered_up  :1;
	bool    left_lop_m_powered_up   :1;
	bool    mono_lop_m_powered_up   :1;
	bool    right_dac_powered_up    :1;
	bool    left_dac_powered_up     :1;
} tlv320aic3106_module_pwr_status_t;

typedef struct
{
	uint8_t _padding0                       :2;
	bool    hprcom_powered_up               :1;
	bool    hplcom_powered_up               :1;
	bool    hprcom_short_circuit_detected   :1;
	bool    hplcom_short_circuit_detected   :1;
	bool    hprout_short_circuit_detected   :1;
	bool    hplout_short_circuit_detected   :1;
} tlv320aic3106_out_drv_short_circuit_status_t;

typedef struct
{
	bool    right_adc_pwr_lower_than_agc_ths    :1;
	bool    left_adc_pwr_lower_than_agc_ths     :1;
	bool    headset_insertion_removal_detected  :1;
	bool    headset_button_press_detected       :1;
	bool    hprcom_short_circuit_detected       :1;
	bool    hplcom_short_circuit_detected       :1;
	bool    hprout_short_circuit_detected       :1;
	bool    hplout_short_circuit_detected       :1;
} tlv320aic3106_sticky_interrupt_flags_t;

typedef struct
{
	bool	right_adc_pwr_lower_than_agc_ths   :1;
	bool	left_adc_pwr_lower_than_agc_ths    :1;
	bool	headset_detected                   :1;
	bool	headset_button_press_detected      :1;
	bool	hprcom_short_circuit_detected      :1;
	bool	hplcom_short_circuit_detected      :1;
	bool	hprout_short_circuit_detected      :1;
	bool	hplout_short_circuit_detected      :1;
} tlv320aic3106_real_time_interrupt_flags_t;

typedef enum
{
	TLV320AIC3106_GPIO1_CFG_DISABLED                                    = 0x00,
	TLV320AIC3106_GPIO1_CFG_ADC_WORD_CLK                                = 0x01,
	TLV320AIC3106_GPIO1_CFG_CLK_MUX_DIV_1                               = 0x02,
	TLV320AIC3106_GPIO1_CFG_CLK_MUX_DIV_2                               = 0x03,
	TLV320AIC3106_GPIO1_CFG_CLK_MUX_DIV_4                               = 0x04,
	TLV320AIC3106_GPIO1_CFG_CLK_MUX_DIV_8                               = 0x05,
	TLV320AIC3106_GPIO1_CFG_SHORT_CIRCUIT_INT                           = 0x06,
	TLV320AIC3106_GPIO1_CFG_AGC_NOISE_INT                               = 0x07,
	TLV320AIC3106_GPIO1_CFG_INPUT                                       = 0x08,
	TLV320AIC3106_GPIO1_CFG_OUTPUT                                      = 0x09,
	TLV320AIC3106_GPIO1_CFG_DIG_MIC_MOD_CLK                             = 0x0A,
	TLV320AIC3106_GPIO1_CFG_WORD_CLK                                    = 0x0B,
	TLV320AIC3106_GPIO1_CFG_BTN_PRESS_INT                               = 0x0C,
	TLV320AIC3106_GPIO1_CFG_HEADSET_DETECT_INT                          = 0x0D,
	TLV320AIC3106_GPIO1_CFG_HEADSET_DETECT_OR_BTN_INT                   = 0x0E,
	TLV320AIC3106_GPIO1_CFG_HEADSET_DETECT_OR_BTN_OR_SHORT_OR_AGC_INT   = 0x0F,
} tlv320aic3106_gpio1_cfg_t;

typedef enum
{
	TLV320AIC3106_GPIO1_CLK_MUX_OUT_PLL         = 0x00,
	TLV320AIC3106_GPIO1_CLK_MUX_OUT_CLK_DIV_MUX = 0x01,
} tlv320aic3106_gpio1_clk_mux_out_t;

typedef enum
{
	TLV320AIC3106_GPIOX_INT_DURATION_CTRL_SNGL_ACT_HIGH_2MS = 0x00,
	TLV320AIC3106_GPIOX_INT_DURATION_CTRL_CONT_PULSES       = 0x01,
} tlv320aic3106_gpiox_int_duration_ctrl_t;

typedef struct
{
	uint8_t                                 gpio1_out_val           :1;
	uint8_t                                 gpio1_in_val            :1;
	tlv320aic3106_gpiox_int_duration_ctrl_t gpio1_int_duration_ctrl :1;
	tlv320aic3106_gpio1_clk_mux_out_t       gpio1_clk_mux_out       :1;
	tlv320aic3106_gpio1_cfg_t               gpio1_cfg               :4;
} tlv320aic3106_gpio1_ctrl_t;

typedef enum
{
	TLV320AIC3106_GPIO2_CFG_DISABLED                                    = 0x00,
	TLV320AIC3106_GPIO2_CFG_HEADSET_DETECT_INT                          = 0x02,
	TLV320AIC3106_GPIO2_CFG_INPUT                                       = 0x03,
	TLV320AIC3106_GPIO2_CFG_OUTPUT                                      = 0x04,
	TLV320AIC3106_GPIO2_CFG_DIG_MIC_IN                                  = 0x05,
	TLV320AIC3106_GPIO2_CFG_BIT_CLK                                     = 0x08,
	TLV320AIC3106_GPIO2_CFG_HEADSET_DETECT_OR_BTN_INT                   = 0x09,
	TLV320AIC3106_GPIO2_CFG_HEADSET_DETECT_OR_BTN_OR_SHORT_OR_AGC_INT   = 0x0A,
	TLV320AIC3106_GPIO2_CFG_SHORT_OR_AGC_INT                            = 0x0B,
	TLV320AIC3106_GPIO2_CFG_HEADSET_DETECT_OR_BTN_OR_SHORT_INT          = 0x0C,
	TLV320AIC3106_GPIO2_CFG_SHORT_INT                                   = 0x0D,
	TLV320AIC3106_GPIO2_CFG_AGC_INT                                     = 0x0E,
	TLV320AIC3106_GPIO2_CFG_BTN_PRESS_INT                               = 0x0F,
} tlv320aic3106_gpio2_cfg_t;

typedef struct
{
	uint8_t                                 _padding0               :1;
	tlv320aic3106_gpiox_int_duration_ctrl_t gpio1_int_duration_ctrl :1;
	uint8_t                                 gpio2_in_val            :1;
	uint8_t                                 gpio2_out_val           :1;
	tlv320aic3106_gpio2_cfg_t               gpio2_cfg               :4;
} tlv320aic3106_gpio2_ctrl_t;

typedef enum
{
	TLV320AIC3106_GPIO_I2C_PIN_CTRL_NO_GPIO = 0x00,
	TLV320AIC3106_GPIO_I2C_PIN_CTRL_INPUT   = 0x01,
	TLV320AIC3106_GPIO_I2C_PIN_CTRL_OUTPUT  = 0x02,
} tlv320aic3106_gpio_i2c_pin_ctrl_t;

typedef struct
{
	uint8_t                             scl_in_val  :1;
	uint8_t                             scl_out_val :1;
	tlv320aic3106_gpio_i2c_pin_ctrl_t   scl_pin_ctrl:2;
	uint8_t                             sda_in_val  :1;
	uint8_t                             sda_out_val :1;
	tlv320aic3106_gpio_i2c_pin_ctrl_t   sda_pin_ctrl:2;
} tlv320aic3106_gpio_ctrl_a_t;

typedef enum
{
	TLV320AIC3106_MFP3_SD_BUS_CFG_AS_SDIN   = 0x00,
	TLV320AIC3106_MFP3_SD_BUS_CFG_AS_MOSI   = 0x01,
} tlv320aic3106_mfp3_sd_bus_cfg_t;

typedef enum
{
	TLV320AIC3106_CODEC_CLKIN_SRC_PLLDIV_OUT    = 0x00,
	TLV320AIC3106_CODEC_CLKIN_SRC_CLKDIV_OUT    = 0x01,
} tlv320aic3106_codec_clkin_src_t;;

typedef struct
{
	tlv320aic3106_codec_clkin_src_t     codec_clkin_src             :1;
	uint8_t                             mfp3_as_output_val          :1;
	bool                                mfp3_as_output_en           :1;
	uint8_t                             mfp3_in_val                 :1;
	tlv320aic3106_mfp3_sd_bus_cfg_t     mfp3_sd_bus_cfg             :1;
	bool                                mfp3_as_input_en            :1;
	uint8_t                             i2c_address_pin1_val_at_rst :1;
	uint8_t                             i2c_address_pin0_val_at_rst :1;
} tlv320aic3106_gpio_ctrl_b_t;

typedef struct
{
	bool                  enable;
	tlv320aic3106_pll_p_t p;
	tlv320aic3106_pll_q_t q;
	uint8_t               j;
	uint16_t              d;
	tlv320aic3106_pll_r_t r;
} tlv320aic3106_pll_config_t;

typedef enum
{
	TLV320AIC3106_CLK_IN_SRC_MCLK   = 0x00,
	TLV320AIC3106_CLK_IN_SRC_GPIO2  = 0x01,
	TLV320AIC3106_CLK_IN_SRC_BCLK   = 0x02,
} tlv320aic3106_clk_in_src_t;

typedef enum
{
	TLV320AIC3106_PLL_CLK_DIV_N_16  = 0x00,
	TLV320AIC3106_PLL_CLK_DIV_N_17  = 0x01,
	TLV320AIC3106_PLL_CLK_DIV_N_2   = 0x02,
	TLV320AIC3106_PLL_CLK_DIV_N_3   = 0x03,
	TLV320AIC3106_PLL_CLK_DIV_N_4   = 0x04,
	TLV320AIC3106_PLL_CLK_DIV_N_5   = 0x05,
	TLV320AIC3106_PLL_CLK_DIV_N_6   = 0x06,
	TLV320AIC3106_PLL_CLK_DIV_N_7   = 0x07,
	TLV320AIC3106_PLL_CLK_DIV_N_8   = 0x08,
	TLV320AIC3106_PLL_CLK_DIV_N_9   = 0x09,
	TLV320AIC3106_PLL_CLK_DIV_N_10  = 0x0A,
	TLV320AIC3106_PLL_CLK_DIV_N_11  = 0x0B,
	TLV320AIC3106_PLL_CLK_DIV_N_12  = 0x0C,
	TLV320AIC3106_PLL_CLK_DIV_N_13  = 0x0D,
	TLV320AIC3106_PLL_CLK_DIV_N_14  = 0x0E,
	TLV320AIC3106_PLL_CLK_DIV_N_15  = 0x0F,
} tlv320aic3106_pll_clk_div_n_t;

typedef struct
{
	tlv320aic3106_pll_clk_div_n_t   pllclk_div_n    :4;
	tlv320aic3106_clk_in_src_t      pllclk_in_src   :2;
	tlv320aic3106_clk_in_src_t      clkdiv_in_src   :2;
} tlv320aic3106_clk_gen_ctrl_t;

typedef enum
{
	TLV320AIC3106_LEFT_AGC_TIME_REG_SELECT_26   = 0x00,
	TLV320AIC3106_LEFT_AGC_TIME_REG_SELECT_THIS = 0x01,
} tlv320aic3106_left_agc_time_reg_select_t;

typedef enum
{
	TLV320AIC3106_RIGHT_AGC_TIME_REG_SELECT_29      = 0x00,
	TLV320AIC3106_RIGHT_AGC_TIME_REG_SELECT_THIS    = 0x01,
} tlv320aic3106_right_agc_time_reg_select_t;

typedef enum
{
	TLV320AIC3106_BASELINE_AGC_ATTCK_TIME_7MS   = 0x00,
	TLV320AIC3106_BASELINE_AGC_ATTCK_TIME_8MS   = 0x01,
	TLV320AIC3106_BASELINE_AGC_ATTCK_TIME_10MS  = 0x02,
	TLV320AIC3106_BASELINE_AGC_ATTCK_TIME_11MS  = 0x03,
} tlv320aic3106_baseline_agc_attck_time_t;

typedef enum
{
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_1    = 0x00,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_2    = 0x01,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_4    = 0x02,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_8    = 0x03,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_16   = 0x04,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_32   = 0x05,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_64   = 0x06,
	TLV320AIC3106_MULT_FACTOR_FOR_BASELINE_AGC_128  = 0x07,
} tlv320aic3106_mult_factor_for_baseline_agc_t;

typedef struct
{
	uint8_t                                         _padding0                   :2;
	tlv320aic3106_mult_factor_for_baseline_agc_t    mult_factor_for_baseline_agc:3;
	tlv320aic3106_baseline_agc_attck_time_t         baseline_agc_attck_time     :2;
	tlv320aic3106_left_agc_time_reg_select_t        attck_time_reg_select       :1;
} tlv320aic3106_left_agc_new_prog_attck_time_t;

typedef enum
{
	TLV320AIC3106_BASELINE_AGC_DECAY_TIME_50MS  = 0x00,
	TLV320AIC3106_BASELINE_AGC_DECAY_TIME_150MS = 0x01,
	TLV320AIC3106_BASELINE_AGC_DECAY_TIME_250MS = 0x02,
	TLV320AIC3106_BASELINE_AGC_DECAY_TIME_350MS = 0x03,
} tlv320aic3106_baseline_agc_decay_time_t;

typedef struct
{
	uint8_t                                         _padding0                   :2;
	tlv320aic3106_mult_factor_for_baseline_agc_t    mult_factor_for_baseline_agc:3;
	tlv320aic3106_baseline_agc_decay_time_t         left_agc_decay_time         :2;
	tlv320aic3106_left_agc_time_reg_select_t        decay_time_reg_select       :1;
} tlv320aic3106_left_agc_new_prog_decay_time_t;

typedef struct
{
	uint8_t                                         _padding0                   :2;
	tlv320aic3106_mult_factor_for_baseline_agc_t    mult_factor_for_baseline_agc:3;
	tlv320aic3106_baseline_agc_attck_time_t         baseline_agc_attck_time     :2;
	tlv320aic3106_right_agc_time_reg_select_t       attck_time_reg_select       :1;
} tlv320aic3106_right_agc_new_prog_attck_time_t;

typedef struct
{
	uint8_t                                         _padding0                   :2;
	tlv320aic3106_mult_factor_for_baseline_agc_t    mult_factor_for_baseline_agc:3;
	tlv320aic3106_baseline_agc_decay_time_t         right_agc_decay_time        :2;
	tlv320aic3106_right_agc_time_reg_select_t       decay_time_reg_select       :1;
} tlv320aic3106_right_agc_new_prog_decay_time_t;

typedef enum
{
	TLV320AIC3106_ADC_DEC_FILTER_LEFT_AND_RIGHT_DIG_MIC = 0x00,
	TLV320AIC3106_ADC_DEC_FILTER_LEFT_DIG_RIGHT_ANA_MIC = 0x01,
	TLV320AIC3106_ADC_DEC_FILTER_RIGHT_DIG_LEFT_ANA_MIC = 0x02,
	TLV320AIC3106_ADC_DEC_FILTER_LEFT_AND_RIGHT_ANA_MIC = 0x03,
} tlv320aic3106_adc_dec_filter_t;

typedef struct
{
	bool                            i2c_bus_error_detected          :1;
	uint8_t                         _padding0                       :1;
	bool                            no_i2c_error_detect_en          :1;
	bool                            prog_filter_conn_to_adc_out     :1;
	tlv320aic3106_adc_dec_filter_t  adc_dec_filter_cfg              :2;
	bool                            right_high_pass_prog_coeff_en   :1;
	bool                            left_high_pass_prog_coeff_en    :1;
} tlv320aic3106_new_adc_dig_path_and_i2c_cond_t;

typedef struct
{
	bool    line1lp_routed_to_left_lop  :1;
	bool    line1lm_routed_to_left_lom  :1;
	bool    line2lp_routed_to_left_lop  :1;
	bool    line2lm_routed_to_left_lom  :1;
	bool    line1rp_routed_to_right_lop :1;
	bool    line1rm_routed_to_right_lom :1;
	bool    line2rp_routed_to_right_lop :1;
	bool    line2rm_routed_to_right_lom :1;
} tlv320aic3106_passive_ana_sig_bypass_sel_pd_t;

typedef enum
{
	TLV320AIC3106_DAC_QUIESCENT_CURRENT_DEFAULT     = 0x00,
	TLV320AIC3106_DAC_QUIESCENT_CURRENT_1_5_DAC_REF = 0x01,
	TLV320AIC3106_DAC_QUIESCENT_CURRENT_2_DAC_REF   = 0x03,
} tlv320aic3106_dac_quiescent_current_t;

typedef struct
{
	uint8_t                                 _padding0               :6;
	tlv320aic3106_dac_quiescent_current_t   dac_quiescent_current   :2;
} tlv320aic3106_dac_quiescent_current_adj_t;

typedef struct
{
	tlv320aic3106_pll_prog_reg_a_t                  pll_prog_reg_a;
	tlv320aic3106_gpio_ctrl_b_t                     gpio_ctrl_b;
	tlv320aic3106_passive_ana_sig_bypass_sel_pd_t   passive_ana_sig_bypass_sel_pd;
	tlv320aic3106_ac_pwr_and_out_drv_ctrl_t         ac_pwr_and_out_drv_ctrl;
	tlv320aic3106_dac_dig_volume_ctrl_t             dac_dig_volume_ctrl;
	tlv320aic3106_x_to_y_volume_ctrl_t              dac_r1_to_right_lop;
	tlv320aic3106_x_to_y_volume_ctrl_t              dac_l1_to_left_lop;
	tlv320aic3106_x_out_lvl_ctrl_t                  right_lop_m_out_lvl_ctrl;
	tlv320aic3106_x_out_lvl_ctrl_t                  left_lop_m_out_lvl_ctrl;
} tlv320aic3106_config_t;

typedef enum
{
	TLV320AIC3106_EVT_TYPE_ERROR,
	TLV320AIC3106_EVT_TYPE_RX_MODULE_PWR_STATUS
} tlv320aic3106_evt_type_t;

typedef struct
{
	tlv320aic3106_t        * p_tlv320aic3106;
	tlv320aic3106_evt_type_t type;
	union
	{
		ret_code_t  err_code;
		tlv320aic3106_module_pwr_status_t * p_module_pwr_status;
	} params;
} tlv320aic3106_evt_t;

typedef void (* tlv320aic3106_evt_handler_t)(tlv320aic3106_evt_t * p_event);

struct tlv320aic3106_s
{
	const dk_twi_mngr_t           * p_dk_twi_mngr_instance; /**< Pointer to TWI manager instance. */
	uint8_t                         i2c_address;
	tlv320aic3106_evt_handler_t     evt_handler;
	tlv320aic3106_active_page_t     active_page;
	tlv320aic3106_config_t          config;
};

#define TLV320AIC3106_DEF(_name, _p_dk_twi_mngr_instance, _i2c_address) \
static tlv320aic3106_t _name =                                          \
{                                                                       \
	.p_dk_twi_mngr_instance = _p_dk_twi_mngr_instance,                  \
	.i2c_address            = _i2c_address,                             \
	.evt_handler            = NULL,                                     \
	.active_page            = TLV320AIC3106_ACTIVE_PAGE_0               \
};

ret_code_t tlv320aic3106_init(tlv320aic3106_t             * p_tlv320aic3106,
                              tlv320aic3106_evt_handler_t   evt_handler);

ret_code_t tlv320aic3106_soft_rst(tlv320aic3106_t * p_tlv320aic3106);

ret_code_t tlv320aic3106_set_passive_ana_sig_bypass_sel_pd(tlv320aic3106_t * p_tlv320aic3106,
                                                           tlv320aic3106_passive_ana_sig_bypass_sel_pd_t * p_bypass);

ret_code_t tlv320aic3106_set_line1_bypass(tlv320aic3106_t * p_tlv320aic3106,
                                          bool enable);

ret_code_t tlv320aic3106_pll_init(tlv320aic3106_t * p_tlv320aic3106,
                                  tlv320aic3106_pll_config_t * p_pll_config);

ret_code_t tlv320aic3106_pll_enable(tlv320aic3106_t * p_tlv320aic3106,
                                    bool enable);

ret_code_t tlv320aic3106_set_pll_prog_reg_a(tlv320aic3106_t * p_tlv320aic3106,
                                            tlv320aic3106_pll_prog_reg_a_t * p_pll_prog_reg_a);

ret_code_t tlv320aic3106_set_pll_prog_reg_b(tlv320aic3106_t * p_tlv320aic3106,
                                            tlv320aic3106_pll_prog_reg_b_t * p_pll_prog_reg_b);

ret_code_t tlv320aic3106_set_pll_d(tlv320aic3106_t * p_tlv320aic3106, uint16_t d_value);

ret_code_t tlv320aic3106_set_datapath(tlv320aic3106_t * p_tlv320aic3106,
                                      tlv320aic3106_datapath_setup_t * p_datapath_setup);

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_a(tlv320aic3106_t * p_tlv320aic3106,
                                                             tlv320aic3106_audio_ser_data_interface_ctrl_a_t * p_audio_ser_data_interface_ctrl_a);

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_b(tlv320aic3106_t * p_tlv320aic3106,
                                                             tlv320aic3106_audio_ser_data_interface_ctrl_b_t * p_audio_ser_data_interface_ctrl_b);

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_c(tlv320aic3106_t * p_tlv320aic3106,
                                                             uint8_t offset);

ret_code_t tlv320aic3106_set_pll_r(tlv320aic3106_t * p_tlv320aic3106,
                                   tlv320aic3106_pll_r_t pll_r);

ret_code_t tlv320aic3106_set_digital_filter_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_audio_codec_digital_filter_ctrl_t * p_dig_filter_ctrl);

ret_code_t tlv320aic3106_set_headset_btn_press_detect_b(tlv320aic3106_t * p_tlv320aic3106,
                                                        tlv320aic3106_headset_btn_press_detect_b_t * p_headset_btn_press_detect_b);

ret_code_t tlv320aic3106_set_ac_pwr_and_out_drv_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_ac_pwr_and_out_drv_ctrl_t * p_ac_pwr_and_out_drv_ctrl);

ret_code_t tlv320aic3106_set_dac_pwr(tlv320aic3106_t * p_tlv320aic3106,
                                     bool right_dac_en,
                                     bool left_dac_en);

ret_code_t tlv320aic3106_set_hi_pwr_out_stage_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                   tlv320aic3106_hi_pwr_out_stage_ctrl_t * p_hi_pwr_out_stage_ctrl);

ret_code_t tlv320aic3106_set_dac_out_switch_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_dac_out_switch_ctrl_t * p_dac_out_switch_ctrl);

ret_code_t tlv320aic3106_set_out_drv_pop_reduction(tlv320aic3106_t * p_tlv320aic3106,
                                                   tlv320aic3106_out_drv_pop_reduction_t * p_out_drv_pop_reduction);

ret_code_t tlv320aic3106_set_dac_dig_volume_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_dac_dig_volume_ctrl_t * p_dac_dig_volume_ctrl);

ret_code_t tlv320aic3106_set_dac_mute(tlv320aic3106_t * p_tlv320aic3106,
                                      bool mute);

ret_code_t tlv320aic3106_set_right_dac_dig_volume_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_dac_dig_volume_ctrl_t * p_dac_dig_volume_ctrl);

ret_code_t tlv320aic3106_set_left_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl);

ret_code_t tlv320aic3106_set_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl);

ret_code_t tlv320aic3106_set_left_lop_m_out_mute(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool mute);

ret_code_t tlv320aic3106_set_left_lop_m_out_pwr_en(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool pwr_en);

ret_code_t tlv320aic3106_set_dac_r1_to_right_lop(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_r1_to_right_lop);

ret_code_t tlv320aic3106_set_dac_l1_to_left_lop(tlv320aic3106_t * p_tlv320aic3106,
                                                tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_l1_to_left_lop);

ret_code_t tlv320aic3106_set_dac_r1_to_right_lop_volume(tlv320aic3106_t * p_tlv320aic3106,
                                                        uint8_t volume);

ret_code_t tlv320aic3106_set_dac_l1_to_left_lop_volume(tlv320aic3106_t * p_tlv320aic3106,
                                                       uint8_t volume);

ret_code_t tlv320aic3106_set_dac_x1_to_lop(tlv320aic3106_t * p_tlv320aic3106,
                                        tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_to_lop);

ret_code_t tlv320aic3106_set_right_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl);

ret_code_t tlv320aic3106_set_right_lop_m_out_mute(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool mute);

ret_code_t tlv320aic3106_set_right_lop_m_out_pwr_en(tlv320aic3106_t * p_tlv320aic3106,
                                                    bool pwr_en);

ret_code_t tlv320aic3106_set_gpio_ctrl_b(tlv320aic3106_t * p_tlv320aic3106,
                                         tlv320aic3106_gpio_ctrl_b_t * p_gpio_ctrl_b);

ret_code_t tlv320aic3106_set_clkin_src(tlv320aic3106_t * p_tlv320aic3106,
                                       tlv320aic3106_codec_clkin_src_t clkin_src);

ret_code_t tlv320aic3106_set_clk_gen_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                          tlv320aic3106_clk_gen_ctrl_t * p_clk_gen_ctrl);

ret_code_t tlv320aic3106_set_dac_quiescient_current(tlv320aic3106_t * p_tlv320aic3106,
                                                    tlv320aic3106_dac_quiescent_current_adj_t * p_dac_quiscient);

ret_code_t tlv320aic3106_get_module_power_status(tlv320aic3106_t * p_tlv320aic3106);

ret_code_t tlv320aic3106_debug(tlv320aic3106_t * p_tlv320aic3106);

#ifdef __cplusplus
}
#endif

#endif //TLV320AIC3106_H
