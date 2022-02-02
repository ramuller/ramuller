#include <vfs_api.h>
#include <FSImpl.h>
#include <FS.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>


const char* host = "esp32";
const char* ssid = "bvb11";
const char* password = "Stenbocke 14b";

WebServer server(80);

int barCarcount = 1;
int barCarIsRunning = false;

#define IM_A_SLAVE 1
#include "2-wheel-car.h"

// #include <Serial.h>


/*
 * 
 * *************************************************
 * 
 * BAR CAR
 *  
 * 
 */


/*
 * Login page
 */

const char* loginIndex = R""""(
 <form name='loginForm'>
    <table width='20%' bgcolor='A09F9F' align='center'>
        <tr>
            <td colspan=2>
                <center><font size=4><b>ESP32 Login Page</b></font></center>
                <br>
            </td>
            <br>
            <br>
        </tr>
        <tr>
             <td>Username:</td>
             <td><input type='text' size=25 name='userid'><br></td>
        </tr>
        <br>
        <br>
        <tr>
            <td>Password:</td>
            <td><input type='Password' size=25 name='pwd'><br></td>
            <br>
            <br>
        </tr>
        <tr>
            <td><input type='submit' onclick='check(this.form)' value='Login'></td>
        </tr>
        <tr>
            <td><input type='submit' onclick='start(this.form)' value='Start Barcar'></td>
        </tr>
        <tr>
            <td><input type="checkbox" name='flower' value='rose' checked='checked' /></td>
        </tr>
    </table>
</form>
<script>
    function check(form)
    {
    if(form.userid.value=='admin' && form.pwd.value=='admin')
    {
    window.open('/serverIndex')
    }
    if(form.userid.value=='run')
    {
    window.open('/runBarcar')
    }
    else
    {
     alert('Error Password or Username')/*displays error message*/
    }
    }
    function start(form)
    {
    window.open('/runBarcar')
    /*  alert('No clue how to start barcar') */
    }
</script>;
)"""";


const char* startIndex = R""""(
  <form name='startForm'>
    <table width='20%' bgcolor='A09F9F' align='center'>
        <tr>
            <td><input type='submit' onclick='start(this.form)' value='Start Barcar'></td>
        </tr>
        <tr>
            <td><input type='submit' onclick='update(this.form)' value='Update SW'></td>
        </tr>
        <tr>
            <td><input type='submit' onclick='testing(this.form)' value='testing'></td>
        </tr>
    </table>
  </form>
  <script>
  function start(form)
  {
    window.close()
    window.open('/startBarCar')
  }
  function update(form)
  {
    window.open('/serverIndex')
  }
  function testing(form)
  {
    window.open('/testing')
  }
</script>;
)"""";

const char* stopIndex = R""""(
  <form name='startForm'>
    <table width='20%' bgcolor='A0F000' align='center'>
        <tr>
            <td><input type='submit' onclick='stop(this.form)' value='Stop Barcar'></td>
        </tr>
        <tr>
            <td><input type='submit' onclick='update(this.form)' value='Update SW'></td>
        </tr>
        <tr>
            <td><input type='submit' onclick='testing(this.form)' value='testing'></td>
        </tr>
    </table>
  </form>
  <script>
  function stop(form)
  {
    window.close()
    window.open('/')
  }
  function update(form)
  {
    window.open('/serverIndex')
  }
  function testing(form)
  {
    window.alert('Bar car will run intil you press')
  }
</script>;
)"""";


/*
 * Server Index Page
 */

const char* serverIndex = R""""(
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js'></script>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
   <input type='file' name='update'>
        <input type='submit' value='Update'>
   </form>
 <form name='loginForm'>
    <table width='20%' bgcolor='A09F9F' align='center'>
        <tr>
            <td><input type='submit' onclick='start(this.form)' value='Start Barcar'></td>
        </tr>
    </table>
</form>
<div id='prg'>progress: 0%</div>
 <script>
  $('form').submit(function(e){
  e.preventDefault();
  var form = $('#upload_form')[0];
  var data = new FormData(form);
   $.ajax({
  url: '/update',
  type: 'POST',
  data: data,
  contentType: false,
  processData:false,
  xhr: function() {
  var xhr = new window.XMLHttpRequest();
  xhr.upload.addEventListener('progress', function(evt) {
  if (evt.lengthComputable) {
  var per = evt.loaded / evt.total;
  $('#prg').html('progress: ' + Math.round(per*100) + '%');
  }
  }, false);
  return xhr;
  },
  success:function(d, s) {
  console.log('success!')
 },
 error: function (a, b, c) {
 }
 });
 });
 </script>;
)"""";

void barCarFunc(boolean);
void barCarFunc(boolean state) {
  Serial.printf("Set running state to %d\n", state);
  barCarIsRunning = state;
}

void homePage(){
    server.send(200, "text/html", startIndex);
    Serial.printf("Home page clicked stopping barcar\n");
    barCarFunc(0);
}

void startBarCar(){
    server.send(200, "text/html", stopIndex);
    Serial.printf("Home page clicked starting barcar\n");
    barCarFunc(1);
}

void testing(){
    server.send(200, "text/plain", "hello from esp32!");
    Serial.printf("Testing\n");
}

/*
 * setup function
 */
void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started.");
  /*return index page which is stored in serverIndex */
  server.on("/", homePage);
  server.on("/startBarCar", startBarCar);
  server.on("/testing", testing);
  
  server.on("/bla", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", startIndex);
    barCarFunc(1);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
    barCarFunc(0);
  });
  server.on("/startIndex2", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", stopIndex);
    barCarFunc(0);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    barCarFunc(0);
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
  slave_setup();
  // Serial.println(loginIndex);
}
void loop(void) {
  server.handleClient();
  if (barCarIsRunning != 0){
    Serial.printf("Counter : %d\n", barCarcount++);
    slave_loop();
  } else {
    motorCTRL(0, STOP, 0);
  }
  delay(1);
}
