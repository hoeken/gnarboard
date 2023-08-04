#include "server.h"
#include "prefs.h"
#include "wifi.h"
#include "ota.h"
#include "adc.h"
#include "fans.h"

String board_name = "Yarrboard";

//username / password for websocket authentication
String app_user = "admin";
String app_pass = "admin";
bool require_login = true;

//keep track of our authenticated clients
const byte clientLimit = 8;
uint32_t authenticatedClientIDs[clientLimit];

AsyncWebSocket ws("/ws");
AsyncWebServer server(80);

//for tracking our message loop
int messageInterval = 250;
unsigned long previousMessageMillis = 0;
unsigned int lastHandledMessages = 0;

void websocket_setup()
{
  //look up our board name
  if (preferences.isKey("boardName"))
    board_name = preferences.getString("boardName");

  //look up our username/password
  if (preferences.isKey("app_user"))
    app_user = preferences.getString("app_user");
  if (preferences.isKey("app_pass"))
    app_pass = preferences.getString("app_pass");
  if (preferences.isKey("require_login"))
    require_login = preferences.getBool("require_login");

  //config for our websocket server
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //we are only serving static files - big cache
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setCacheControl("max-age=2592000");
  server.begin();
}

void websocket_loop()
{
  //sometimes websocket clients die badly.
  ws.cleanupClients();

  //lookup our info periodically
  int messageDelta = millis() - previousMessageMillis;
  if (messageDelta >= messageInterval)
  {
    //read and send out our json update
    sendUpdate();
  
    //how fast are we?
    //Serial.print(messageDelta);
    //Serial.print("ms | msg/s: ");
    //Serial.print(handledMessages - lastHandledMessages);
    //Serial.println();

    //for keeping track.
    lastHandledMessages = handledMessages;
    previousMessageMillis = millis();
  }
}

void sendToAll(char * jsonString)
{
  //send the message to all authenticated clients.
  if (require_login)
  {
    for (byte i=0; i<clientLimit; i++)
      if (authenticatedClientIDs[i])
        if (ws.availableForWrite(authenticatedClientIDs[i]))
          ws.text(authenticatedClientIDs[i], jsonString);
  }
  //nope, just sent it to all.
  else {
    if (ws.availableForWriteAll())
      ws.textAll(jsonString);
    else
      Serial.println("[socket] queue full");
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[socket] #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("[socket] #%u disconnected\n", client->id());

      //clear this guy from our authenticated list.
      if (require_login)
        for (byte i=0; i<clientLimit; i++)
          if (authenticatedClientIDs[i] == client->id())
            authenticatedClientIDs[i] = 0;

      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len, client);
      break;
    case WS_EVT_PONG:
      Serial.printf("[socket] #%u pong", client->id());
      break;
    case WS_EVT_ERROR:
      Serial.printf("[socket] #%u error", client->id());
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    char jsonBuffer[MAX_JSON_LENGTH];
    handleReceivedJSON((char*)data, jsonBuffer, YBP_MODE_WEBSOCKET, client->id());

    if (client->canSend())
      ws.text(client->id(), jsonBuffer);
  }
}

bool isClientLoggedIn(uint32_t client_id)
{
  //also only if enabled
  if (!require_login)
    return true;

  //are they in our auth array?
  for (byte i=0; i<clientLimit; i++)
    if (authenticatedClientIDs[i] == client_id)
      return true;

  //default to fail then.
  return false;  
}

bool logClientIn(uint32_t client_id)
{
  byte i;
  for (i=0; i<clientLimit; i++)
  {
    //did we find an empty slot?
    if (authenticatedClientIDs[i] == 0)
    {
      authenticatedClientIDs[i] = client_id;
      break;
    }

    //are we already authenticated?
    if (authenticatedClientIDs[i] == client_id)
      break;
  }

  //did we not find a spot?
  if (i == clientLimit)
  {
    AsyncWebSocketClient* client = ws.client(client_id);
    client->close();

    return false;
  }

  return true;
}

void sendUpdate()
{
  char jsonBuffer[MAX_JSON_LENGTH];
  generateUpdateJSON(jsonBuffer);
  sendToAll(jsonBuffer);
}

void sendOTAProgressUpdate(float progress, int partition)
{
  char jsonBuffer[MAX_JSON_LENGTH];
  generateOTAProgressUpdateJSON(jsonBuffer, progress, partition);
  sendToAll(jsonBuffer);
}

void sendOTAProgressFinished()
{
  char jsonBuffer[MAX_JSON_LENGTH];
  generateOTAProgressFinishedJSON(jsonBuffer);
  sendToAll(jsonBuffer);
}