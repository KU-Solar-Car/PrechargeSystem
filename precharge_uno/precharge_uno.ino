int output_pin = 10;
int input_pin = 3;
void setup() {
  // put your setup code here, to run once:
pinMode (output_pin, OUTPUT);
pinMode (input_pin, INPUT);
digitalWrite (output_pin, HIGH);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
if
(digitalRead (input_pin)== HIGH) 
{
  digitalWrite (output_pin, LOW);
  Serial.print("Made it\n"); 
}
else
{
  digitalWrite (output_pin, HIGH);
}
delay (1);

}
