CC ?= gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic -I include
BUILD_DIR := build
TARGET := $(BUILD_DIR)/asteria-sim
CORE_SOURCES := src/mission.c src/commands.c src/event_log.c src/health.c src/scheduler.c src/storage.c src/subsystems.c src/telemetry.c src/drivers/adc.c src/drivers/radio.c src/drivers/sensor.c
SOURCES := src/main.c $(CORE_SOURCES)
TEST_TARGET := $(BUILD_DIR)/asteria-tests

.PHONY: all run test clean

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) sim/commands.txt

$(TEST_TARGET): tests/test_mission.c $(CORE_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/test_mission.c $(CORE_SOURCES) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)
