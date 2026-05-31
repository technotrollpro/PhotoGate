#include "PhotoGate.h"

// Статические указатели на объекты
static PhotoGate* gateInstance1 = nullptr;
static PhotoGate* gateInstance2 = nullptr;

// Глобальные функции-обертки для прерываний
void gateInterruptPin2() {
  if (gateInstance1) gateInstance1->_interruptHandler();
}

void gateInterruptPin3() {
  if (gateInstance2) gateInstance2->_interruptHandler();
}

// --------------------------------------------------
// КОНСТРУКТОР
// --------------------------------------------------
PhotoGate::PhotoGate(int sensorPin, int gateId) {
  _sensorPin = sensorPin;
  _gateId = gateId;
  _triggered = false;
  _lastTriggerTime = 0;
  _triggerCount = 0;
  _debounceMicros = 5000;  // 5 мс по умолчанию
}

// --------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ
// --------------------------------------------------
void PhotoGate::begin() {
  pinMode(_sensorPin, INPUT_PULLUP);
  
  // Регистрируем обработчик прерывания в зависимости от пина
  if (_sensorPin == 2) {
    gateInstance1 = this;
    attachInterrupt(digitalPinToInterrupt(_sensorPin), gateInterruptPin2, FALLING);
  } 
  else if (_sensorPin == 3) {
    gateInstance2 = this;
    attachInterrupt(digitalPinToInterrupt(_sensorPin), gateInterruptPin3, FALLING);
  }
}

// --------------------------------------------------
// ОБРАБОТЧИК ПРЕРЫВАНИЯ (приватный)
// --------------------------------------------------
void PhotoGate::_interruptHandler() {
  unsigned long now = micros();
  
  // Антидребезг
  if ((now - _lastTriggerTime) < _debounceMicros) {
    return;
  }
  
  _lastTriggerTime = now;
  _triggered = true;
}

// --------------------------------------------------
// ПРОВЕРКА ПРЕРЫВАНИЯ
// --------------------------------------------------
bool PhotoGate::check() {
  if (_triggered) {
    _triggered = false;
    _triggerCount++;
    return true;
  }
  return false;
}

// --------------------------------------------------
// ПРОВЕРКА СОСТОЯНИЯ ДАТЧИКА
// --------------------------------------------------
bool PhotoGate::isBlocked() {
  return digitalRead(_sensorPin) == LOW;
}

// --------------------------------------------------
// ПОЛУЧИТЬ СЧЕТЧИК СО СБРОСОМ
// --------------------------------------------------
int PhotoGate::getCount() {
  int count = _triggerCount;
  _triggerCount = 0;
  return count;
}

// --------------------------------------------------
// ПОЛУЧИТЬ СЧЕТЧИК БЕЗ СБРОСА
// --------------------------------------------------
int PhotoGate::peekCount() {
  return _triggerCount;
}

// --------------------------------------------------
// СБРОС СЧЕТЧИКА
// --------------------------------------------------
void PhotoGate::resetCount() {
  _triggerCount = 0;
}

// --------------------------------------------------
// ПОЛУЧИТЬ ID
// --------------------------------------------------
int PhotoGate::getId() {
  return _gateId;
}

// --------------------------------------------------
// УСТАНОВИТЬ ID
// --------------------------------------------------
void PhotoGate::setId(int id) {
  _gateId = id;
}

// --------------------------------------------------
// УСТАНОВИТЬ ЗАДЕРЖКУ АНТИДРЕБЕЗГА
// --------------------------------------------------
void PhotoGate::setDebounceMicros(unsigned long micros) {
  _debounceMicros = micros;
}