# OpenDAC HD
OpenDAC HD is an open source digital to analog converter for high definition audio. With a 127dB SNR DAC chip and premium low noise opamps, it delivers excellent sound quality.

Technical specifications:
* Diana Navarro analog signal path
 * PCM1792A DAC
 * OPA1612 IV converter opamps
 * TPA6120A2 headphone driver opamps
* Active offset correction
* Instrumentation grade power supply
 * Low ESR Japanese capacitors
 * 8 voltage rails
* 3 headphone outputs
* DIX9211 input processor
* Fiber (Toslink) and isolated coax S/PDIF inputs
* Auxiliary microphone preamp for headsets
* RGB sample rate indicator
 * Yellow: no signal or unsupported input
 * Red: <44.1kHz
 * Green: 44.1kHz
 * Cyan: 48kHz
 * Blue: 88.2kHz
 * Violet: 96kHz
 * White: 176.4kHz or 192kHz
* Shielded aluminum and stainless steel case

Here is how OpenDAC HD compares to a high end Intel DX79SI motherboard when driving a set of common 32 ohm headphones:
![scope screenshots](https://github.com/NiHaoMike/OpenDAC-HD/raw/master/OpenDAC_comparison.png)

## FAQ
* What headphones does OpenDAC HD support?
 * Almost all headphones are directly supported by OpenDAC HD. The IV converter resistors may need to be increased in value to get sufficient volume for very high impedance headphones.
 * Electrostatic headphones will require an external amplifier.
* Does OpenDAC HD support surround sound?
 * This is a complicated question because proper headphones are always stereo and as such, surround sound through headphones is handled by HRTF. Thus, it depends on whether or not the source supports surround through headphones.
* Does OpenDAC HD support Mac/Xbox/whatever?
 * OpenDAC HD works with any source device that outputs PCM audio over S/PDIF.
 * Source devices that do not have S/PDIF but do support USB audio (e.g. most laptops) can be connected via a USB to S/PDIF converter.
 * Source devices that do not have S/PDIF but do have HDMI can be connected via a HDMI audio extractor. The S/PDIF output on some HDMI monitors and TVs can also be used if PCM output is selected.
* Is OpenDAC HD good for gaming?
 * Yes. It has much less than a millisecond of latency.
* Can OpenDAC HD be used with speakers?
 * An external amplifier will be required for speaker use. Note that few consumer analog amplifiers are good enough to take advantage of OpenDAC HD's high quality and thus a digital amplifier is generally a better choice.
* Does OpenDAC HD support Dolby/DTS/DSD?
 * No. OpenDAC HD focuses on being a low cost, high quality DAC for music listening, which predominantly is based around PCM.
 * Dolby and DTS require DSPs for decoding, which would considerably increase cost and complexity. Ultimately, a DAC that supports Dolby or DTS will convert it to PCM so you might as well do the conversion in the source device.
 * The DAC chip supports DSD so feel free to try modifying the design to support it.
* Are PCB layouts available?
 * None are available at this time because a PCB has not been made for OpenDAC HD.
* My MP3s sound much worse with OpenDAC HD.
 * That points to bad encoding or a bitrate too low to accurately represent the content. Unfortunately, high quality audio hardware is a double edged sword - what brings out the details and enrichens the enjoyment of fine music will also uncover compression artifacts that were previously hidden by poor hardware.
* Does 96kHz or 192kHz really offer any audio quality advantages over 44.1kHz or 48kHz?
 * This is a highly controversial topic. Do your own research and listening tests and come to your own conclusion. Note that OpenDAC HD still offers a substantial audio quality improvement with "standard definition" audio.

## About us
We love high end HD audio.  
But we do not believe high end HD audio should come at a high end price.  
Nor should true HD audio be enjoyed only by the rich, the elite.  
We believe that HD audio should be available to anyone with some electronics knowledge.  
We embrace the open hardware, DIY movement.  
Not only are we best friends, we strive to be music's best friend.  
![OpenDAC team](https://github.com/NiHaoMike/OpenDAC-HD/raw/master/OpenDAC_team.jpg)
