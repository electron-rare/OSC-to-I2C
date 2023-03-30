
// Fonction server.addCallback("/corbac/gd1", &gd1);
// definir numero de servo   osc_value[N° SERVO] pour avoir le  1st argument(int32)

void gd1(OSCMessage &msg, int addrOffset)
{
  servo_select = 0;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd2(OSCMessage &msg, int addrOffset)
{
  servo_select = 1;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd3(OSCMessage &msg, int addrOffset)
{
  servo_select = 10;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd4(OSCMessage &msg, int addrOffset)
{
  servo_select = 11;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd5(OSCMessage &msg, int addrOffset)
{
  servo_select = 16;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd6(OSCMessage &msg, int addrOffset)
{
  servo_select = 17;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd7(OSCMessage &msg, int addrOffset)
{
  servo_select = 26;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void gd8(OSCMessage &msg, int addrOffset)
{
  servo_select = 27;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}

void c1(OSCMessage &msg, int addrOffset)
{
  servo_select = 4;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c2(OSCMessage &msg, int addrOffset)
{
  servo_select = 5;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c3(OSCMessage &msg, int addrOffset)
{
  servo_select = 6;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c4(OSCMessage &msg, int addrOffset)
{
  servo_select = 7;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c5(OSCMessage &msg, int addrOffset)
{
  servo_select = 12;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c6(OSCMessage &msg, int addrOffset)
{
  servo_select = 13;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c7(OSCMessage &msg, int addrOffset)
{
  servo_select = 14;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c8(OSCMessage &msg, int addrOffset)
{
  servo_select = 15;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c9(OSCMessage &msg, int addrOffset)
{
  servo_select = 20;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c10(OSCMessage &msg, int addrOffset)
{
  servo_select = 21;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c11(OSCMessage &msg, int addrOffset)
{
  servo_select = 22;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c12(OSCMessage &msg, int addrOffset)
{
  servo_select = 23;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c13(OSCMessage &msg, int addrOffset)
{
  servo_select = 28;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c14(OSCMessage &msg, int addrOffset)
{
  servo_select = 29;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c15(OSCMessage &msg, int addrOffset)
{
  servo_select = 30;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}
void c16(OSCMessage &msg, int addrOffset)
{
  servo_select = 31;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
  servo_send(servo_select);
}

void sheep(OSCMessage &msg, int addrOffset)
{
  servo_select = 32;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void badger(OSCMessage &msg, int addrOffset)
{
  servo_select = 33;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow1(OSCMessage &msg, int addrOffset)
{
  servo_select = 34;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
void cow2(OSCMessage &msg, int addrOffset)
{
  servo_select = 35;
  osc_value[servo_select] = int(msg.getFloat(0) * 1000);
}
