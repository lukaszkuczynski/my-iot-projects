build:
	docker build -t panlukasz/raspberry-weather .

run:
	docker run --rm -ti --env-file rasp-env.env --device /dev/i2c-1 panlukasz/raspberry-weather
