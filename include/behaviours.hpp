#pragma once

#include "esp32-hal-adc.h"
#include "esp_adc/adc_continuous.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "freertos/projdefs.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <vector>
#include <behaviour.hpp>
#include <complex>
#include <cstdint>
#include <complex>
#include <utility>
#include <numeric>

class OriginalBehaviour : public Behaviour {
	String name() {
		return "Original (presque)";
	}

	void run() {
		for (int i=0;i<10;i++) {
			Ron();
			Gon();
			wait(50);
			Roff();
			Goff();
			wait(50);
		}
		for (int i=0;i<10;i++) {
			Yon();
			wait(50);
			Yoff();
			wait(50);
		}
		for (int i=0;i<5;i++) {
			Ron();
			wait(50);
			Roff();
			wait(50);
		}
		for (int i=0;i<5;i++) {
			Gon();
			wait(50);
			Goff();
			wait(50);
		}

	}
public:
	static int id;
};

class FeuBehaviour : public Behaviour {
	String name() {
		return "Feu tricolore";
	}

	void run() {
		Ron();
		wait(30000);
		Roff();
		Gon();
		wait(30000);
		Goff();
		Yon();
		wait(3000);
		Yoff();
	}
public:
	static int id;
};

class FeuOrangeBehaviour : public Behaviour {
	String name() {
		return "Feu tricolore en travaux";
	}

	void run() {
		Yon();
		wait(1000);
		Yoff();
		wait(1000);
	}
public:
	static int id;
};

class Clem1Behaviour : public Behaviour {
	String name() {
		return "Clem 1 - Bounce";
	}

	void run() {
		unsigned long t = 100;
		Ron();
		wait(t);
		Yon();
		wait(t);
		Roff();
		wait(t);
		Gon();
		wait(t);
		Yoff();
		wait(t);
		Yon();
		wait(t);
		Goff();
		wait(t);
		Ron();
		wait(t);
		Yoff();
	}
public:
	static int id;
};

class Clem2Behaviour : public Behaviour {
	String name() {
		return "Clem 2 - Random";
	}

	void run() {
		long r = random(11);
		if (r < 5)
			Ron();
		else
			Roff();

		wait(100);

		r = random(11);
		if (r < 5)
			Yon();
		else
			Yoff();

		wait(100);

		r = random(11);
		if (r < 5)
			Gon();
		else
			Goff();

		wait(100);
	}
public:
	static int id;
};

class Clem3Behaviour : public Behaviour {
	String name() {
		return "Clem 3 - Random 2";
	}

	void run() {
		long t = random(30,400);

		Ron();
		wait(t);
		Roff();
		wait(t);
		Yon();
		wait(t);
		Yoff();
		wait(t);
		Gon();
		wait(t);
		Goff();
	}
public:
	static int id;
};


class Clem4Behaviour : public Behaviour {
	String name() {
		return "Clem 4 - Random 3";
	}

	void run() {
		Ron();
		wait(random(20,400));
		Roff();
		wait(random(20,400));
		Yon();
		wait(random(20,400));
		Yoff();
		wait(random(20,400));
		Gon();
		wait(random(20,400));
		Goff();
	}
public:
	static int id;
};

class Clem5Behaviour : public Behaviour {
	String name() {
		return "Clem 5 - Bug";
	}

	void run() {
		Ron();
		wait(1000);
		Roff();
		wait(400);
		Yon();
		wait(random(20,400));
		Yoff();
		wait(80);
		Ron();
		wait(200);
		Roff();
		wait(400);
		Gon();
		wait(random(20,400));
		Goff();
	}
public:
	static int id;
};

class Vincent1Behaviour : public Behaviour {
	String name() {
		return "Vincent 1 - Caca v2";
	}

	void run() {
		static bool up = false;
		if (random(4) == 0)
			up ^= true;

		if (up) {
			Ron();
			wait(100);
			Roff();
			Yon();
			wait(100);
			Yoff();
			Gon();
			wait(100);
			Goff();
		} else {
			Gon();
			wait(100);
			Goff();
			Yon();
			wait(100);
			Yoff();
			Ron();
			wait(100);
			Roff();
		}
	}
public:
	static int id;
};

class MatthiasMusicBehaviour : public Behaviour {
    static constexpr uint32_t SAMPLE_FREQUENCY {50000};
    static constexpr uint8_t ANALOG_SOUND_PIN {36};
    static constexpr size_t SIGNAL_BUFFER_LENGTH {1024};

	// 50-250Hz
	static constexpr size_t LOW_INDEX {1};

	// 300-4000Hz
	static constexpr size_t MEDIUM_INDEX {(300 * SIGNAL_BUFFER_LENGTH) / SAMPLE_FREQUENCY};

	// 4000-10000Hz
	static constexpr size_t HIGH_INDEX {(4000 * SIGNAL_BUFFER_LENGTH) / SAMPLE_FREQUENCY};

	static constexpr size_t END_INDEX {(10000 * SIGNAL_BUFFER_LENGTH) / SAMPLE_FREQUENCY};

	std::array<adc_continuous_data_t, SIGNAL_BUFFER_LENGTH> time_buffer;
	std::array<std::complex<float>, SIGNAL_BUFFER_LENGTH> freq_buffer;
	adc_continuous_handle_t handle;

	float last_low;
	float last_medium;
	float last_high;

	// how fast the new values for low, medium and high affect the new last values
	static constexpr float alpha {0.2f};

	String name() {
		return "Matthias Music";
	}

	int fft() {
		// copy time signal
		for (size_t i = 0; i < time_buffer.size(); ++i) {
		    if (!time_buffer[i].valid) {
				Serial.println("error during use of ADC read values");
				return 1;
			}
			freq_buffer[i] = {static_cast<float>(time_buffer[i].raw_data), 0.0f};
		}

		// bit reversal
		for (size_t i = 1, j = 0; i < SIGNAL_BUFFER_LENGTH; ++i) {
			size_t bit = SIGNAL_BUFFER_LENGTH >> 1;

			for (; j & bit; bit >>= 1)
				j ^= bit;

			j ^= bit;

			if (i < j)
				std::swap(freq_buffer[i], freq_buffer[j]);
		}

		// radix-2 FFT
		for (std::size_t len = 2; len <= SIGNAL_BUFFER_LENGTH; len <<= 1) {
			const float angle = -2.0f * PI / static_cast<float>(len);
			const auto w_step = std::polar(1.0f, angle);

			for (size_t i = 0; i < SIGNAL_BUFFER_LENGTH; i += len) {
				std::complex<float> w = 1.0f;

				for (std::size_t j = 0; j < len / 2; ++j) {
					const auto u = freq_buffer[i + j];
					const auto v = freq_buffer[i + j + len / 2] * w;

					freq_buffer[i + j] = u + v;
					freq_buffer[i + j + len / 2] = u - v;

					w *= w_step;
				}
			}
		}

		return 0;
	}

	void run() {
        uint32_t n_smaples {time_buffer.size()};
		esp_err_t ret = adc_continuous_read_parse(handle, time_buffer.data(), time_buffer.size(), &n_smaples, 100);

		if (ret != ESP_OK) {
		    Serial.println("error during adc_continuous_read_parse");
		    return;
		}

		{
			if (fft()) return;

			float low = std::accumulate(
				freq_buffer.begin() + LOW_INDEX,
				freq_buffer.begin() + MEDIUM_INDEX,
				0.0f,
				[](float sum, const std::complex<float>& c) { return sum + std::norm(c); }
			) / (MEDIUM_INDEX - LOW_INDEX);

			float medium = std::accumulate(
				freq_buffer.begin() + MEDIUM_INDEX,
				freq_buffer.begin() + HIGH_INDEX,
				0.0f,
				[](float sum, const std::complex<float>& c) { return sum + std::norm(c); }
			) / (HIGH_INDEX - MEDIUM_INDEX);

			float high = std::accumulate(
				freq_buffer.begin() + HIGH_INDEX,
				freq_buffer.begin() + END_INDEX,
				0.0f,
				[](float sum, const std::complex<float>& c) { return sum + std::norm(c); }
			) / (END_INDEX - HIGH_INDEX);

			Goff();
			Yoff();
			Roff();

			last_low = (1-alpha) * last_low + alpha * low;
			last_medium = (1-alpha) * last_medium + alpha * medium;
			last_high = (1-alpha) * last_high + alpha * high;

			Serial.println(low);
			Serial.println(medium);
			Serial.println(high);

			if (last_low >= 200000) Gon();
			if (last_medium >= 200000) Yon();
			if (last_high >= 250000) Ron();

			Serial.print("after whole loop ");
			Serial.println(millis());
		}

		Serial.println("all good man");
	}
public:
    MatthiasMusicBehaviour() : handle {nullptr}, last_low{}, last_medium{}, last_high{} {
    	Serial.println("Init Sound");

    	pinMode(ANALOG_SOUND_PIN, INPUT);

    	adc_continuous_handle_cfg_t adc_handle_config {
           	.max_store_buf_size = SIGNAL_BUFFER_LENGTH * sizeof(uint16_t),
    		.conv_frame_size = SIGNAL_BUFFER_LENGTH * sizeof(uint16_t) / 4,
    	};


        adc_digi_pattern_config_t pattern {
            .atten = ADC_ATTEN_DB_12,
            .channel = ADC_CHANNEL_0,
            .unit = ADC_UNIT_1,
            .bit_width = ADC_BITWIDTH_12,
        };
    	adc_continuous_config_t adc_config {
    	    .pattern_num = 1,
           	.adc_pattern = &pattern,
           	.sample_freq_hz = SAMPLE_FREQUENCY,
           	.conv_mode = ADC_CONV_SINGLE_UNIT_1,
           	.format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    	};

    	ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_handle_config, &handle));
    	ESP_ERROR_CHECK(adc_continuous_config(handle, &adc_config));
        ESP_ERROR_CHECK(adc_continuous_start(handle));
    }

	static int id;
};
