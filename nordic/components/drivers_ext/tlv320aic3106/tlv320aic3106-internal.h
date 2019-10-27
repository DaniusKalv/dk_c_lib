/**
 * @file        tlv320aic3106-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320AIC3106 audio CODEC driver internal macros.
 * @version     0.1
 * @date        2019-10-02
 * 
 * @copyright   Copyright (c) 2019 Danius Kalvaitis
 * 
 */
 
#ifndef TLV320AIC3106_INTERNAL_H
#define TLV320AIC3106_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define TLV320AIC3106_PAGE0_BIT     0x00
#define TLV320AIC3106_PAGE1_BIT     0x80

#define TLV320AIC3106_PAGE0(_reg_address) _reg_address | TLV320AIC3106_PAGE0_BIT
#define TLV320AIC3106_PAGE1(_reg_address) _reg_address | TLV320AIC3106_PAGE1_BIT

#define TLV320AIC3106_PAGE_SELECT   0x00

#define TLV320AIC3106_SOFT_RST      TLV320AIC3106_PAGE0(1)

/** @brief Value to be written to TLV320AIC3106_SOFT_RST register to perform a 
self clearing software reset. */
#define TLV320AIC3106_SOFT_RST_VAL                          0x80

#define TLV320AIC3106_SAMPLE_RATE_SELECT                    TLV320AIC3106_PAGE0(2)

#define TLV320AIC3106_PLL_PROG_REG_A                        TLV320AIC3106_PAGE0(3)
#define TLV320AIC3106_PLL_PROG_REG_B                        TLV320AIC3106_PAGE0(4)
#define TLV320AIC3106_PLL_PROG_REG_C                        TLV320AIC3106_PAGE0(5)
#define TLV320AIC3106_PLL_PROG_REG_D                        TLV320AIC3106_PAGE0(6)

#define TLV320AIC3106_DATAPATH_SETUP                        TLV320AIC3106_PAGE0(7)

#define TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_A       TLV320AIC3106_PAGE0(8)
#define TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_B       TLV320AIC3106_PAGE0(9)
#define TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_C       TLV320AIC3106_PAGE0(10)

#define TLV320AIC3106_AUDIO_CODEC_OVERFLOW_FLAG             TLV320AIC3106_PAGE0(11)

#define TLV320AIC3106_AUDIO_CODEC_DIGITAL_FILTER_CTRL       TLV320AIC3106_PAGE0(12)

#define TLV320AIC3106_HEADSET_BTN_PRESS_DETECT_A            TLV320AIC3106_PAGE0(13)
#define TLV320AIC3106_HEADSET_BTN_PRESS_DETECT_B            TLV320AIC3106_PAGE0(14)

#define TLV320AIC3106_LEFT_ADC_PGA_GAIN_CTRL                TLV320AIC3106_PAGE0(15)
#define TLV320AIC3106_RIGHT_ADC_PGA_GAIN_CTRL               TLV320AIC3106_PAGE0(16)

#define TLV320AIC3106_MIC3L_R_TO_LEFT_ADC_CTRL              TLV320AIC3106_PAGE0(17)
#define TLV320AIC3106_MIC3L_R_TO_RIGHT_ADC_CTRL             TLV320AIC3106_PAGE0(18)

#define TLV320AIC3106_LINE1L_TO_LEFT_ADC_CTRL               TLV320AIC3106_PAGE0(19)
#define TLV320AIC3106_LINE2L_TO_LEFT_ADC_CTRL               TLV320AIC3106_PAGE0(20)

#define TLV320AIC3106_LINE1R_TO_LEFT_ADC_CTRL               TLV320AIC3106_PAGE0(21)
#define TLV320AIC3106_LINE1R_TO_RIGHT_ADC_CTRL              TLV320AIC3106_PAGE0(22)

#define TLV320AIC3106_LINE2R_TO_RIGHT_ADC_CTRL              TLV320AIC3106_PAGE0(23)
#define TLV320AIC3106_LINE1L_TO_RIGHT_ADC_CTRL              TLV320AIC3106_PAGE0(24)

#define TLV320AIC3106_MICBIAS_CTRL                          TLV320AIC3106_PAGE0(25)

#define TLV320AIC3106_LEFT_AGC_CTRL_A                       TLV320AIC3106_PAGE0(26)
#define TLV320AIC3106_LEFT_AGC_CTRL_B                       TLV320AIC3106_PAGE0(27)
#define TLV320AIC3106_LEFT_AGC_CTRL_C                       TLV320AIC3106_PAGE0(28)

#define TLV320AIC3106_RIGHT_AGC_CTRL_A                      TLV320AIC3106_PAGE0(29)
#define TLV320AIC3106_RIGHT_AGC_CTRL_B                      TLV320AIC3106_PAGE0(30)
#define TLV320AIC3106_RIGHT_AGC_CTRL_C                      TLV320AIC3106_PAGE0(31)

#define TLV320AIC3106_LEFT_AGC_GAIN                         TLV320AIC3106_PAGE0(32)
#define TLV320AIC3106_RIGHT_AGC_GAIN                        TLV320AIC3106_PAGE0(33)

#define TLV320AIC3106_LEFT_AGC_NOISE_GATE_DEBOUNCE          TLV320AIC3106_PAGE0(34)
#define TLV320AIC3106_RIGHT_AGC_NOISE_GATE_DEBOUNCE         TLV320AIC3106_PAGE0(35)

#define TLV320AIC3106_ADC_FLAG                              TLV320AIC3106_PAGE0(36)

#define TLV320AIC3106_AC_PWR_AND_OUT_DRV_CTRL               TLV320AIC3106_PAGE0(37)
#define TLV320AIC3106_HI_PWR_OUT_DRV_CTRL                   TLV320AIC3106_PAGE0(38)

#define TLV320AIC3106_HI_PWR_OUT_STAGE_CTRL                 TLV320AIC3106_PAGE0(40)
#define TLV320AIC3106_DAC_OUT_SWITCH_CTRL                   TLV320AIC3106_PAGE0(41)
#define TLV320AIC3106_OUT_DRV_POP_REDUCTION                 TLV320AIC3106_PAGE0(42)

#define TLV320AIC3106_LEFT_DAC_DIG_VOLUME_CTRL              TLV320AIC3106_PAGE0(43)
#define TLV320AIC3106_RIGHT_DAC_DIG_VOLUME_CTRL             TLV320AIC3106_PAGE0(44)

#define TLV320AIC3106_LINE2L_TO_HPLOUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(45)
#define TLV320AIC3106_PGA_L_TO_HPLOUT_VOLUME_CTRL           TLV320AIC3106_PAGE0(46)
#define TLV320AIC3106_DAC_L1_TO_HPLOUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(47)
#define TLV320AIC3106_LINE2R_TO_HPLOUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(48)
#define TLV320AIC3106_PGA_R_TO_HPLOUT_VOLUME_CTRL           TLV320AIC3106_PAGE0(49)
#define TLV320AIC3106_DAC_R1_TO_HPLOUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(50)

#define TLV320AIC3106_HPLOUT_OUT_LVL_CTRL                   TLV320AIC3106_PAGE0(51)

#define TLV320AIC3106_LINE2L_TO_HPLCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(52)
#define TLV320AIC3106_PGA_L_TO_HPLCOM_VOLUME_CTRL           TLV320AIC3106_PAGE0(53)
#define TLV320AIC3106_DAC_L1_TO_HPLCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(54)
#define TLV320AIC3106_LINE2R_TO_HPLCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(55)
#define TLV320AIC3106_PGA_R_TO_HPLCOM_VOLUME_CTRL           TLV320AIC3106_PAGE0(56)
#define TLV320AIC3106_DAC_R1_TO_HPLCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(57)

#define TLV320AIC3106_HPLCOM_OUT_LVL_CTRL                   TLV320AIC3106_PAGE0(58)

#define TLV320AIC3106_LINE2L_TO_HPROUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(59)
#define TLV320AIC3106_PGA_L_TO_HPROUT_VOLUME_CTRL           TLV320AIC3106_PAGE0(60)
#define TLV320AIC3106_DAC_L1_TO_HPROUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(61)
#define TLV320AIC3106_LINE2R_TO_HPROUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(62)
#define TLV320AIC3106_PGA_R_TO_HPROUT_VOLUME_CTRL           TLV320AIC3106_PAGE0(63)
#define TLV320AIC3106_DAC_R1_TO_HPROUT_VOLUME_CTRL          TLV320AIC3106_PAGE0(64)

#define TLV320AIC3106_HPROUT_OUT_LVL_CTRL                   TLV320AIC3106_PAGE0(65)

#define TLV320AIC3106_LINE2L_TO_HPRCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(66)
#define TLV320AIC3106_PGA_L_TO_HPRCOM_VOLUME_CTRL           TLV320AIC3106_PAGE0(67)
#define TLV320AIC3106_DAC_L1_TO_HPRCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(68)
#define TLV320AIC3106_LINE2R_TO_HPRCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(69)
#define TLV320AIC3106_PGA_R_TO_HPRCOM_VOLUME_CTRL           TLV320AIC3106_PAGE0(70)
#define TLV320AIC3106_DAC_R1_TO_HPRCOM_VOLUME_CTRL          TLV320AIC3106_PAGE0(71)

#define TLV320AIC3106_HPRCOM_OUT_LVL_CTRL                   TLV320AIC3106_PAGE0(72)

#define TLV320AIC3106_LINE2L_TO_MONO_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(73)
#define TLV320AIC3106_PGA_L_TO_MONO_LOP_M_VOLUME_CTRL       TLV320AIC3106_PAGE0(74)
#define TLV320AIC3106_DAC_L1_TO_MONO_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(75)
#define TLV320AIC3106_LINE2R_TO_MONO_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(76)
#define TLV320AIC3106_PGA_R_TO_MONO_LOP_M_VOLUME_CTRL       TLV320AIC3106_PAGE0(77)
#define TLV320AIC3106_DAC_R1_TO_MONO_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(78)

#define TLV320AIC3106_MONO_LOP_M_OUT_LVL_CTRL               TLV320AIC3106_PAGE0(79)

#define TLV320AIC3106_LINE2L_TO_LEFT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(80)
#define TLV320AIC3106_PGA_L_TO_LEFT_LOP_M_VOLUME_CTRL       TLV320AIC3106_PAGE0(81)
#define TLV320AIC3106_DAC_L1_TO_LEFT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(82)
#define TLV320AIC3106_LINE2R_TO_LEFT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(83)
#define TLV320AIC3106_PGA_R_TO_LEFT_LOP_M_VOLUME_CTRL       TLV320AIC3106_PAGE0(84)
#define TLV320AIC3106_DAC_R1_TO_LEFT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(85)

#define TLV320AIC3106_LEFT_LOP_M_OUT_LVL_CTRL               TLV320AIC3106_PAGE0(86)

#define TLV320AIC3106_LINE2L_TO_RIGHT_LOP_M_VOLUME_CTRL     TLV320AIC3106_PAGE0(87)
#define TLV320AIC3106_PGA_L_TO_RIGHT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(88)
#define TLV320AIC3106_DAC_L1_TO_RIGHT_LOP_M_VOLUME_CTRL     TLV320AIC3106_PAGE0(89)
#define TLV320AIC3106_LINE2R_TO_RIGHT_LOP_M_VOLUME_CTRL     TLV320AIC3106_PAGE0(90)
#define TLV320AIC3106_PGA_R_TO_RIGHT_LOP_M_VOLUME_CTRL      TLV320AIC3106_PAGE0(91)
#define TLV320AIC3106_DAC_R1_TO_RIGHT_LOP_M_VOLUME_CTRL     TLV320AIC3106_PAGE0(92)

#define TLV320AIC3106_RIGHT_LOP_M_OUT_LVL_CTRL              TLV320AIC3106_PAGE0(92)

#define TLV320AIC3106_MODULE_PWR_STATUS                     TLV320AIC3106_PAGE0(94)
#define TLV320AIC3106_OUT_DRV_SHORT_CIRCUIT_STATUS          TLV320AIC3106_PAGE0(95)
#define TLV320AIC3106_STICKY_INTERRUPT_FLAGS                TLV320AIC3106_PAGE0(96)
#define TLV320AIC3106_REAL_TIME_INTERRUPT_FLAGS             TLV320AIC3106_PAGE0(97)

#define TLV320AIC3106_GPIO1_CTRL                            TLV320AIC3106_PAGE0(98)
#define TLV320AIC3106_GPIO2_CTRL                            TLV320AIC3106_PAGE0(99)

#define TLV320AIC3106_GPIO_CTRL_A                           TLV320AIC3106_PAGE0(100)
#define TLV320AIC3106_GPIO_CTRL_B                           TLV320AIC3106_PAGE0(101)

#define TLV320AIC3106_CLK_GEN_CTRL                          TLV320AIC3106_PAGE0(102)

#define TLV320AIC3106_LEFT_AGC_NEW_PROG_ATTCK_TIME          TLV320AIC3106_PAGE0(103)
#define TLV320AIC3106_LEFT_AGC_NEW_PROG_DECAY_TIME          TLV320AIC3106_PAGE0(104)
#define TLV320AIC3106_RIGHT_AGC_NEW_PROG_ATTCK_TIME         TLV320AIC3106_PAGE0(105)
#define TLV320AIC3106_RIGHT_AGC_NEW_PROG_DECAY_TIME         TLV320AIC3106_PAGE0(106)

#define TLV320AIC3106_NEW_ADC_DIG_PATH_AND_I2C_COND         TLV320AIC3106_PAGE0(107)
#define TLV320AIC3106_PASSIVE_ANA_SIG_BYPASS_SEL_PD         TLV320AIC3106_PAGE0(108)

#define TLV320AIC3106_DAC_QUIESCENT_CURRENT_ADJ             TLV320AIC3106_PAGE0(109)

#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N0_MSB             TLV320AIC3106_PAGE1(1)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N0_LSB             TLV320AIC3106_PAGE1(2)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N1_MSB             TLV320AIC3106_PAGE1(3)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N1_LSB             TLV320AIC3106_PAGE1(4)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N2_MSB             TLV320AIC3106_PAGE1(5)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N2_LSB             TLV320AIC3106_PAGE1(6)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N3_MSB             TLV320AIC3106_PAGE1(7)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N3_LSB             TLV320AIC3106_PAGE1(8)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N4_MSB             TLV320AIC3106_PAGE1(9)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N4_LSB             TLV320AIC3106_PAGE1(10)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N5_MSB             TLV320AIC3106_PAGE1(11)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_N5_LSB             TLV320AIC3106_PAGE1(12)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D1_MSB             TLV320AIC3106_PAGE1(13)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D1_LSB             TLV320AIC3106_PAGE1(14)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D2_MSB             TLV320AIC3106_PAGE1(15)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D2_LSB             TLV320AIC3106_PAGE1(16)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D4_MSB             TLV320AIC3106_PAGE1(17)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D4_LSB             TLV320AIC3106_PAGE1(18)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D5_MSB             TLV320AIC3106_PAGE1(19)
#define TLV320AIC3106_LEFT_CH_EFF_FILTER_D5_LSB             TLV320AIC3106_PAGE1(20)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_N0_MSB         TLV320AIC3106_PAGE1(21)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_N0_LSB         TLV320AIC3106_PAGE1(22)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_N1_MSB         TLV320AIC3106_PAGE1(23)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_N1_LSB         TLV320AIC3106_PAGE1(24)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_D1_MSB         TLV320AIC3106_PAGE1(25)
#define TLV320AIC3106_LEFT_CH_DE_EMPH_FILTER_D1_LSB         TLV320AIC3106_PAGE1(26)

#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N0_MSB            TLV320AIC3106_PAGE1(27)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N0_LSB            TLV320AIC3106_PAGE1(28)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N1_MSB            TLV320AIC3106_PAGE1(29)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N1_LSB            TLV320AIC3106_PAGE1(30)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N2_MSB            TLV320AIC3106_PAGE1(31)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N2_LSB            TLV320AIC3106_PAGE1(32)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N3_MSB            TLV320AIC3106_PAGE1(33)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N3_LSB            TLV320AIC3106_PAGE1(34)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N4_MSB            TLV320AIC3106_PAGE1(35)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N4_LSB            TLV320AIC3106_PAGE1(36)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N5_MSB            TLV320AIC3106_PAGE1(37)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_N5_LSB            TLV320AIC3106_PAGE1(38)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D1_MSB            TLV320AIC3106_PAGE1(39)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D1_LSB            TLV320AIC3106_PAGE1(40)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D2_MSB            TLV320AIC3106_PAGE1(41)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D2_LSB            TLV320AIC3106_PAGE1(42)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D4_MSB            TLV320AIC3106_PAGE1(43)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D4_LSB            TLV320AIC3106_PAGE1(44)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D5_MSB            TLV320AIC3106_PAGE1(45)
#define TLV320AIC3106_RIGHT_CH_EFF_FILTER_D5_LSB            TLV320AIC3106_PAGE1(46)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_N0_MSB        TLV320AIC3106_PAGE1(47)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_N0_LSB        TLV320AIC3106_PAGE1(48)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_N1_MSB        TLV320AIC3106_PAGE1(49)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_N1_LSB        TLV320AIC3106_PAGE1(50)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_D1_MSB        TLV320AIC3106_PAGE1(51)
#define TLV320AIC3106_RIGHT_CH_DE_EMPH_FILTER_D1_LSB        TLV320AIC3106_PAGE1(52)

#define TLV320AIC3106_3D_ATTENUATION_COEFF_MSB              TLV320AIC3106_PAGE1(53)
#define TLV320AIC3106_3D_ATTENUATION_COEFF_LSB              TLV320AIC3106_PAGE1(54)

#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_N0_MSB    TLV320AIC3106_PAGE1(65)
#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_N0_LSB    TLV320AIC3106_PAGE1(66)
#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_N1_MSB    TLV320AIC3106_PAGE1(67)
#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_N1_LSB    TLV320AIC3106_PAGE1(68)
#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_D1_MSB    TLV320AIC3106_PAGE1(69)
#define TLV320AIC3106_LEFT_CH_ADC_HIGHPASS_FILTER_D1_LSB    TLV320AIC3106_PAGE1(70)

#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_N0_MSB   TLV320AIC3106_PAGE1(71)
#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_N0_LSB   TLV320AIC3106_PAGE1(72)
#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_N1_MSB   TLV320AIC3106_PAGE1(73)
#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_N1_LSB   TLV320AIC3106_PAGE1(74)
#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_D1_MSB   TLV320AIC3106_PAGE1(75)
#define TLV320AIC3106_RIGHT_CH_ADC_HIGHPASS_FILTER_D1_LSB   TLV320AIC3106_PAGE1(76)

#ifdef __cplusplus
}
#endif

#endif // TLV320AIC3106_INTERNAL_H
