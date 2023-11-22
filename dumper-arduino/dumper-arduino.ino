#include <Arduino.h>

#define MAX_BUFFER 10*1024

char g_buffer[MAX_BUFFER];
uint32_t g_interval = 5000;

char g_cmd[128];

void
setup()
{
  Serial.begin(1000000);
  strcpy(g_buffer, "usage set chunksize -> sent 'chunk=1000', set interval time in ms 'interval=1000'");
}


void
loop()
{
  while (Serial.available())
  {
    char data = Serial.read();
    if (data == '\n')
    {
      char *chunk = strstr(g_cmd, "chunk=");
      if (chunk)
      {
        chunk += strlen("chunk=");        
        int chunk_size = atoi(chunk);
        if (chunk_size > MAX_BUFFER) chunk_size = MAX_BUFFER;
        if (chunk_size <= 0) chunk_size = 1;
        char tmp[16]; memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d-", chunk_size);
        
        memset(g_buffer, 0, sizeof(g_buffer));
        while (strlen(g_buffer) < chunk_size)
        {
          strcat(g_buffer, tmp);
        }
        g_buffer[chunk_size] = '\0';
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

  Serial.println(g_buffer);
  delay(g_interval);
}
