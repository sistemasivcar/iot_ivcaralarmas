<?php
session_start();
$logged = $_SESSION['logged'];
$name = $_SESSION['user_name'];
$user_id = $_SESSION['user_id'];


if($logged == 0){
    echo '<meta http-equiv="refresh" content="1; url=login.php">';
  die();
}

$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");

// TRAIGO TODOS LOS DISPOSITOVOS
$result = $conn->query("SELECT * FROM `devices` WHERE `devices_user_id` = '".$user_id."'");
$devices = $result->fetch_all(MYSQLI_ASSOC);
$count = count($devices);

// TRAIGO TODOS LOS DISPOSITOVOS CONECTADOS
$result2 = $conn->query(" SELECT devices_status FROM `admin_alarmaiot`.`devices` WHERE devices_status = '1' AND devices_user_id = '".$user_id."' ");
$conectados = $result2->fetch_all(MYSQLI_ASSOC);
$count_conectados =count($conectados);

include('application/app/Views/template/head.php');
include('application/app/Views/template/navigation.php');
include('application/app/Views/template/header.php');
include('application/app/Views/template/footer.php');
include('application/app/Views/template/colors.php');
?>
 <div ui-view class="app-body" id="view">


     <?php

    if ($count == 0) {
        ?> <div ui-view class="app-body" id="view"><?php
      echo "<br><br><font size='5'><div style='color:red' align='center'>No tenés <strong>dispositivos...</strong></div></font>";
    }else if ($count == 1){
      include("content/one.php");
    } else if ($count == 2){
      include("content/two.php");
    }else if ($count == 3){
      include("content/three.php");
    }else{ echo "<br><br><font size='5'><div style='color:red' align='center'>Demasiados <strong>dispositivos...</strong></div></font>";
    }
    ?></div>
    <?php


include('application/app/Views/template/end.php');?>

<script src="https://unpkg.com/mqtt/dist/mqtt.min.js"></script>

<script type="text/javascript">

/*
******************************
****** CONEXION  *************
******************************
*/

// connect options
const options = {
      connectTimeout: 4000,

      // Authentication
      clientId: 'gaston',
      username: 'web_client',
      password: '0000',

      keepalive: 60,
      clean: true,
}

var connected = false;

// WebSocket connect url
const WebSocket_URL = 'wss://ivcaralarmas.com:8094/mqtt'

const client = mqtt.connect(WebSocket_URL, options)


client.on('connect', () => {
    console.log('Mqtt conectado por WS! Exito!')
    <?php foreach($devices as $device){?>
    client.subscribe('alarma/values/<?php echo $device['devices_serie'];?>', { qos: 0 }, (error) => {})
    <?php } ?>
})

client.on('message', (topic, message) => {
  console.log('Mensaje recibido bajo tópico: ', topic, ' -> ', message.toString())
  <?php if ($count == 1){ ?>
  process_msg(topic, message);
  <?php }else if ($count == 2){ ?>
  process_msg(topic, message);
  process_msg_2(topic, message);
  <?php }else if ($count == 3){ ?>
  process_msg(topic, message);
  process_msg_2(topic, message);
  process_msg_3(topic, message);
  <?php } ?>
})

client.on('reconnect', (error) => {
    console.log('Error al reconectar', error)
})

client.on('error', (error) => {
    console.log('Error de conexión:', error)
})



</script>
