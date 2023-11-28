#include <Arduino.h>


uint32_t g_interval = 1000;
uint32_t g_chunk_size = 10;
char g_cmd[32];


void
setup()
{
  Serial.begin(115200);
}


void
loop()
{
  while (Serial.available())
  {
    char data = Serial.read();
    data = tolower(data);
    if ((data == '\n') || (data == '\r'))
    {
      char *chunk = strstr(g_cmd, "chunk=");
      if (chunk)
      {
        chunk += strlen("chunk=");        
        g_chunk_size = atoi(chunk);
        if (g_chunk_size <= 0) g_chunk_size = 1;
      }
      char *interval = strstr(g_cmd, "interval=");
      if (interval)
      {
        interval += strlen("interval=");
        g_interval = atoi(interval);
        if (g_interval <= 10) g_interval = 10;
        if (g_interval >= 10000) g_interval = 10000;
      }
      memset(g_cmd, 0, sizeof(g_cmd));
      break;
    }
    g_cmd[strlen(g_cmd)] = data;
  }

  const char *numbers = "0123456789";
  for (int i=0; i<g_chunk_size; i++)
  {
    Serial.write(byte(numbers[i%10]));
  }
  Serial.println();
  //yield();
  
  delay(g_interval);
}
