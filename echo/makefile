# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = echo_server
SOURCE = main.cpp

# Сборка по умолчанию
all: $(TARGET)

# Сборка сервера
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Очистка
clean:
	rm -f $(TARGET)

# Пересборка
rebuild: clean all

# Запуск
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
