#include "beat.h"

bool MaxDetector::update(uint16_t sample, uint16_t time)
{
	if(sample > s)
	{
		s = sample;
		t = time;
		return false;
	}
	else
	{
		last_max_s = max_s;
		last_max_t = max_t;
		max_s = s;
		max_t = t;
		s = 0;
		return true;
	}
}

uint16_t MaxDetector::getDeltaTime(uint16_t scale)
{
	uint16_t delta = 0;
	if(max_t > last_max_t)
	{
		delta = max_t - last_max_t;
	}
	else
	{
		delta = scale - last_max_t + max_t;
	}
	return delta;
}

uint16_t MaxDetector::getSample()
{
	return max_s;
}

uint16_t MaxDetector::getTime()
{
	return max_t;
}

bool Beat::update(uint16_t sample)
{
	if(t >= 2048)
	{
		t = 0;
	}
	
	bool isDetected = false;
	uint8_t i = 0;
	uint16_t interSample = sample;
	uint16_t interTime = t;
	
	t = t + 1;
	
	do
	{
		isDetected = detector[i].update(interSample, interTime);
		if(isDetected)
		{
			if((detector[i].getDeltaTime() >= 2000) | (detector[i].getDeltaTime() <= 300))
			{
				isDetected = false;
			}
		}
		interSample = detector[i].getSample();
		interTime = detector[i].getTime();
		i++;
	} while(isDetected & (i < 8));
	
	if(i == 1)
		return false;
	
	if(isDetected)
		i = 7;
	else
		i = i - 2;
	
	last_beat_s = beat_s;
	last_beat_t = beat_t;
	beat_s = detector[i].getSample();
	beat_t = detector[i].getTime();
	
	if(beat_t > last_beat_t)
	{
		T = beat_t - last_beat_t;
	}
	else
	{
		T = 2048 -last_beat_t + beat_t;
	}
	
	if((T >= 300) & (T <= 2000))
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint8_t Beat::getRate()
{
	if((T >= 300) | (T <= 2000))
	{
		uint16_t time = 0;
		uint8_t r = 0;
		while(time < 60000)
		{
			r = r + 1;
			time = time + T;
		}
		return r;
	}
	else
		return 0xFF;
}


