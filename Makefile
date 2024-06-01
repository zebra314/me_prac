all: build upload
install:
	docker build -t platformio:latest .

	@USB_DEVICE=$$(ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -n 1); \
	if [ -z "$$USB_DEVICE" ]; then \
		echo "No USB device found"; \
		echo "Please connect your device and try again"; \
	else \
		docker create -it --name platformio \
    --mount type=bind,source="$(CURDIR)",target=/me_prac \
		-w /me_prac \
		--device $$USB_DEVICE \
		platformio:latest; \
		echo "PlatformIO container created"; \
		echo "To build the project run 'make build'"; \
		echo "To upload the project run 'make upload'"; \
		echo "To attach to the container run 'make attach'"; \
		echo "To plot the data run 'make plot'"; \
		echo "To clean the container run 'make clean'"; \
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