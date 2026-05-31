#ifndef PHOTOGATE_H
#define PHOTOGATE_H

#include <Arduino.h>

class PhotoGate {
  private:
    int _sensorPin;
    volatile bool _triggered;
    volatile unsigned long _lastTriggerTime;
    int _triggerCount;
    int _gateId;
    unsigned long _debounceMicros;
    
    // Обработчик прерывания 
    void _interruptHandler();
    
  public:
    // Конструктор
    PhotoGate(int sensorPin, int gateId = 0);
    
    // Инициализация (вызывать в setup)
    void begin();
    
    // Проверка прерывания (вызывать в loop)
    bool check();
    
    // Получить текущее состояние датчика
    bool isBlocked();
    
    // Получить счетчик сбросом
    int getCount();
    
    // Получить счетчик без сброса
    int peekCount();
    
    // Сбросить счетчик принудительно
    void resetCount();
    
    // Получить ID ворот
    int getId();
    
    // Установить ID
    void setId(int id);
    
    // Установить задержку антидребезга (мкс)
    void setDebounceMicros(unsigned long micros);
};

#endif