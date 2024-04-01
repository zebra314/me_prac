install:
	docker build -t platformio:latest .

	@USB_DEVICE=$$(ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -n 1); \
	if [ -z "$$USB_DEVICE" ]; then \
		echo "No USB device found"; \
	else \
		docker create -it --name platformio \
    --mount type=bind,source="$(shell pwd)",target=/me_prac \
		-w /me_prac \
		--device $$USB_DEVICE \
		platformio:latest; \
	fi

build:
	docker start platformio
	docker exec platformio /bin/sh -c "pio run"

upload:
	docker start platformio
	docker exec platformio /bin/sh -c "pio run -t upload"

attach:
	docker start platformio
	docker attach platformio

plot:
	./arduino-plotter-listener-linux64/listener
		
clean:
	docker stop platformio
	docker rm platformio
	docker rmi platformio:latest