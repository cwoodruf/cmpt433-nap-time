#!/bin/sh
# these should match my settings for alsa (Cal)
#'Master Playback Volume'
amixer cset numid=1 63
#'Tone Control - Bass'
amixer cset numid=7 0
#'Tone Control - Treble'
amixer cset numid=8 0
#'PCM Playback De-emphasis'
amixer cset numid=10 0
#'Mic Sensitivity'
amixer cset numid=5 1
#'Mic Volume'
amixer cset numid=6 0
#'ADC +6dB Switch'
amixer cset numid=16 off
#'ADC Polarity Switch'
amixer cset numid=17 off
#'Capture Volume'
amixer cset numid=2 1
#'AGC Switch'
amixer cset numid=12 off
#'AGC Target Volume'
amixer cset numid=13 3
#'AGC Timing'
amixer cset numid=14 0
#'Analog1 Volume'
amixer cset numid=3 27
#'Analog2 Volume'
amixer cset numid=4 27
#'DAC +6dB Switch'
amixer cset numid=15 off
#'DAC Polarity Switch'
amixer cset numid=18 off
#'DC Filter Enable Switch'
amixer cset numid=20 off
#'Double Speed Playback Switch'
amixer cset numid=19 off
#'Input Mux'
amixer cset numid=11 0
#'Sound Processing Filter'
amixer cset numid=9 0

