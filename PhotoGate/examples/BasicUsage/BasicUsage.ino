/*
  PhotoGate - Basic Usage Example
  
  Этот пример показывает, как использовать библиотеку PhotoGate
  для работы с двумя датчиками типа "фотоворота".
  
  Подключение:
  - Датчик 1: пин 2
  - Датчик 2: пин 3
  
  Что делает пример:
  - Считает количество срабатываний каждого датчика
  - Выводит события в Serial Monitor
  - Показывает разницу между getCount() и peekCount()
*/

#include <PhotoGate.h>

// Создаём два датчика
// Параметры: (пин, ID)
PhotoGate gateLeft(2, 1);   // левый датчик, ID = 1
PhotoGate gateRight(3, 2);  // правый датчик, ID = 2

// Переменные для хранения общего количества событий
unsigned long totalLeftEvents = 0;
unsigned long totalRightEvents = 0;

// Переменная для мигания встроенным светодиодом
unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("PhotoGate Library - Basic Usage Example"));
  Serial.println(F("========================================="));
  
  // Инициализируем датчики
  gateLeft.begin();
  gateRight.begin();
  
  // Настраиваем антидребезг (опционально)
  // Для оптических пар 5 мс достаточно, для механических - больше
  gateLeft.setDebounceMicros(5000);   // 5 мс
  gateRight.setDebounceMicros(5000);
  
  // Настраиваем встроенный светодиод для визуальной индикации
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println(F("Ready! Trigger sensors to see events..."));
  Serial.println();
}

void loop() {
  // === ОБРАБОТКА СОБЫТИЙ ДАТЧИКОВ ===
  
  // Левый датчик
  if (gateLeft.check()) {
    totalLeftEvents++;
    
    Serial.print(F("[EVENT] Gate Left (ID="));
    Serial.print(gateLeft.getId());
    Serial.print(F(") triggered! Total: "));
    Serial.println(totalLeftEvents);
    
    // Пример работы со счётчиком внутри библиотеки
    int countSinceLastCheck = gateLeft.getCount();  // получим 1
    Serial.print(F("  -> Events since last check: "));
    Serial.println(countSinceLastCheck);
  }
  
  // Правый датчик
  if (gateRight.check()) {
    totalRightEvents++;
    
    Serial.print(F("[EVENT] Gate Right (ID="));
    Serial.print(gateRight.getId());
    Serial.print(F(") triggered! Total: "));
    Serial.println(totalRightEvents);
  }
  
  // === ПРИМЕРЫ ДРУГИХ МЕТОДОВ ===
  
  // 1. Проверка текущего состояния (заблокирован ли луч прямо сейчас)
  static unsigned long lastStatusPrint = 0;
  if (millis() - lastStatusPrint > 5000) {  // каждые 5 секунд
    lastStatusPrint = millis();
    
    Serial.print(F("[STATUS] Left blocked: "));
    Serial.print(gateLeft.isBlocked() ? "YES" : "NO");
    Serial.print(F(", Right blocked: "));
    Serial.println(gateRight.isBlocked() ? "YES" : "NO");
    
    // peekCount() - счётчик без сброса
    Serial.print(F("  -> Left internal counter (peek): "));
    Serial.print(gateLeft.peekCount());
    Serial.print(F(", Right: "));
    Serial.println(gateRight.peekCount());
  }
  
  // 2. Визуальная индикация (мигаем светодиодом при любом срабатывании)
  if (totalLeftEvents > 0 || totalRightEvents > 0) {
    // Если были события — помигаем быстро
    static unsigned long lastEventTime = 0;
    if (millis() - lastEventTime < 500) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      if (millis() - lastEventTime > 1000) {
        lastEventTime = millis();
      }
    }
    
    // Сбросим флаги для мигания
    if (totalLeftEvents > 0 || totalRightEvents > 0) {
      lastEventTime = millis();
      totalLeftEvents = 0;
      totalRightEvents = 0;
    }
  }
  
  // Обычный режим светодиода (медленное мигание, если нет событий)
  if (millis() - lastBlinkTime > 1000) {
    lastBlinkTime = millis();
    ledState = !ledState;
    if (digitalRead(LED_BUILTIN) == LOW) {  // не перебиваем событийную индикацию
      digitalWrite(LED_BUILTIN, ledState);
    }
  }
  
  // Небольшая задержка для стабильности
  delay(10);
}