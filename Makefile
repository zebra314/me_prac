install:
	docker build -t platformio:latest .
	docker run -it --name platformio \
    --mount type=bind,source="$(shell pwd)",target=/me_prac \
		-w /me_prac \
		platformio:latest \

build:
	docker start platformio
	docker exec platformio /bin/sh -c "pio run"

upload:
	docker start platformio
	docker exec platformio /bin/sh -c "pio run -t upload"

attach:
	docker start platformio
	docker attach platformio
		