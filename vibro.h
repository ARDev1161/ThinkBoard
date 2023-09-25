#define VIBRO 22

void vibroInit(){
  pinMode(VIBRO, OUTPUT);
  digitalWrite(VIBRO, HIGH);
  delay(42);
  digitalWrite(VIBRO, LOW);
}
