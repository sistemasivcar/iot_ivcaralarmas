<?php
$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");
$user_id = $_SESSION['user_id'];

// TRAIGO TODOS LOS MODULOS
$resultAlias = $conn->query(" SELECT devices_alias FROM `admin_alarmaiot`.`devices` WHERE devices_user_id = '".$user_id."' ");
$alias1= mysqli_fetch_array($resultAlias);

// TRAIGO TODOS LOS MODULOS CONECTADOS
$resultConn = $conn->query(" SELECT devices_status FROM `admin_alarmaiot`.`devices` WHERE devices_status = '1' AND devices_user_id = '".$user_id."' ");
$conectados = $resultConn->fetch_all(MYSQLI_ASSOC);
$count_conectados =count($conectados);

// SERIE DEL DISPOSITOVO
$resultSerie = $conn->query("SELECT devices_serie FROM `admin_alarmaiot`.`devices` WHERE devices_user_id = '".$user_id."' ");
$serie1= mysqli_fetch_array($resultSerie);
$serie_1 = $serie1['devices_serie'];

// HISTORIAL ACT/DESACT disp 1
$resultHistoric1 = $conn->query(" SELECT act_desact_date, act_desact_command, act_desact_device_serie FROM `admin_alarmaiot`.`act_desact` WHERE act_desact_device_serie = '".$serie_1."'  ORDER BY act_desact_date DESC LIMIT 30");
$data_historial_actdesact_d1= $resultHistoric1->fetch_all(MYSQLI_ASSOC);

$resultHistoric2 = $conn->query(" SELECT robo_res_date, robo_res_command, robo_res_device_serie FROM `admin_alarmaiot`.`robo_res` WHERE robo_res_device_serie = '".$serie_1."'  ORDER BY robo_res_date DESC LIMIT 30");
$data_historial_robores_d1= $resultHistoric2->fetch_all(MYSQLI_ASSOC);

// TRAIGO TODOS ROBOS
$resultRobos = $conn->query(" SELECT robo_res_command FROM `admin_alarmaiot`.`robo_res` WHERE robo_res_command = '1' AND robo_res_device_serie = '".$serie_1."' ");
$robos = $resultRobos->fetch_all(MYSQLI_ASSOC);
$count_robos =count($robos);
 ?>

<div class="dker p-x">
  <div class="row">
    <div class="col-sm-6 push-sm-6">
      <div class="p-y text-center text-sm-right">
        <a href class="inline p-x text-center">
          <span class="h4 block m-0">1</span>
          <small class="text-xs text-muted">Dispositivos</small>
        </a>
        <a href class="inline p-x b-l b-r text-center">
          <span class="h4 block m-0"><?php echo $count_conectados; ?></span>
          <small class="text-xs text-muted">Conectados</small>
        </a>
        <a href class="inline p-x text-center">
          <span class="h4 block m-0"><?php echo $count_robos; ?></span>
          <small class="text-xs text-muted">Robos</small>
        </a>
      </div>
    </div>

    <div class="col-sm-6 pull-sm-6">
      <div class="p-y-md clearfix nav-active-primary">
        <ul class="nav nav-pills nav-sm">
          <li class="nav-item active">
            <a class="nav-link" href data-toggle="tab" data-target="#tab_1"><?php echo $alias1['devices_alias'];?></a>
          </li>

        </ul>
      </div>
    </div>
  </div>
</div>


<div class="padding">
    <div class="row">
      <div class="col-sm-6">
        <div class="box">
          <div class="box-header">
            <h2>Activaciones/Desactivaciones</h2>
          </div>
          <table class="table table-striped b-t">
            <thead>
              <tr>
                <th>FECHA</th>
                <th>IVCAR</th>
              </tr>
            </thead>
            <tbody>
              <?php foreach ($data_historial_actdesact_d1 as $data) {?>
                <tr>
                  <td><?php echo $data['act_desact_date']; ?></td>
                  <td><?php if ($data['act_desact_command'] == '1'){echo "<strong><div style='color:red'>ACTIVADA</div></strong>";}else{echo "<strong><div style='color:lime'>DESACTIVADA</div></strong>";} ?></td>
                </tr>
              <?php } ?>
</tbody>
</table>
</div>
</div>


<div class="col-sm-6">
  <div class="box">
    <div class="box-header">
      <h2>Robos/Restauraciones</h2>
    </div>
    <table class="table table-striped b-t">
      <thead>
        <tr>
          <th>FECHA</th>
          <th>SIRENA</th>
        </tr>
      </thead>
      <tbody>
        <?php foreach ($data_historial_robores_d1 as $data) {?>
          <tr>
            <td><?php echo $data['robo_res_date']; ?></td>
            <td><?php if ($data['robo_res_command'] == '1'){echo "<strong><div style='color:red'>ROBO</div></strong>";}else{echo "<strong><div style='color:lime'>RESTAURADA</div></strong>";} ?></td>
          </tr>
        <?php } ?>
</tbody>
</table>
</div>
</div>


</div>
    </div>




</div>

<!-- ############ PAGE END-->

    </div>
  </div>

<script src="https://unpkg.com/mqtt/dist/mqtt.min.js"></script>

<script type="text/javascript">

/*
******************************
****** PROCESOS  *************
******************************
*/
const act = new Audio('activada.mp3');
const desact = new Audio('desactivada.mp3');
const sirenas = new Audio('alarm.mp3');

function update_values_alarma(estado, sirena, mov, abertura){
  var command = "";

  if (estado == 1){
    estado = 'ACTIVADA';
    command = 'Desactivar';
    $("#img_btn_comand_alarma_id").attr("src","assets/images/desactivar.png");
    $("#img_estado_alarma_id").attr("src","assets/images/activada.png");
  }else if (estado == 0){
    estado = 'DESACTIVADA'
    command = 'Activar';
    $("#img_btn_comand_alarma_id").attr("src","assets/images/activar.png");
    $("#img_estado_alarma_id").attr("src","assets/images/desactivad.png");
  }else if (estado == 2){
    estado = 'ACTIVADA PARCIAL';
    command = 'Desactivar';
    $("#img_btn_comand_alarma_id").attr("src","assets/images/desactivar.png");
    $("#img_estado_alarma_id").attr("src","assets/images/act_parciall.png");
  }


  if (sirena == 1){
    sirenas.play();
    sirena = 'disparadas';
    $("#img_sirena_alarma_id").attr("src","assets/images/sirena_disparada.png");
  }else{
    $("#img_sirena_alarma_id").attr("src","assets/images/sirena_no_disparada.png");
    sirena = 'no disparadas'}


  if (mov == 1){
    $("#img_mov_alarma_id").attr("src","assets/images/mov_run.png");
    mov = 'con movimiento';
  }else{
    $("#img_mov_alarma_id").attr("src","assets/images/mov_stop.png");
    mov = 'sin movimiento'}


  if (abertura == 1){
    $("#img_abertura_alarma_id").attr("src","assets/images/puerta_abierta.png");
    abertura = 'abiertas';
  }else{
    $("#img_abertura_alarma_id").attr("src","assets/images/puerta_cerrada.png");
    abertura = 'cerradas'}



  $("#estado_alarma_id").html(estado);
  $("#txt_sirenas_id").html(sirena);
  $("#txt_mov_id").html(mov);
  $("#txt_abertura_id").html(abertura);
  $("#txt_act_desact_id").html(command);

}

function act_desact(){
  var estado = document.getElementById('txt_act_desact_id');
  if (estado.innerHTML == 'Activar'){
    activar();
  }else{
    desactivar();
  }
}


function activar(){
  client.publish('alarma/command/<?php echo $devices_serie['devices_serie'];?>', 'activar', (error) => {
      console.log(error || 'Mensaje enviado activacion!!!')
    })
    act.play();
}

function desactivar(){
  client.publish('alarma/command/<?php echo $devices_serie['devices_serie'];?>', 'desactivar', (error) => {
      console.log(error || 'Mensaje enviado desac!!!')
    })
    desact.play();

}

function parcial(){
  client.publish('alarma/device/commands', '2', (error) => {
      console.log(error || 'Mensaje enviado parcial!!!')
    })

}


function process_msg(topic, message){

  var sp_topic = topic.split("/");
    //var type = sp_topic[0];
    var query = sp_topic[1];
    //var device = sp_topic[2];

  // ej: "10,11,12"
  if (query == "values"){

    var msg = message.toString();
    var sp_msj = msg.split(",");

    var estado = sp_msj[0];
    var sirena = sp_msj[1];
    var mov = sp_msj[2];
    var abertura = sp_msj[3];
    update_values_alarma(estado,sirena,mov,abertura);

  }
}






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
  process_msg(topic, message);
})

client.on('reconnect', (error) => {
    console.log('Error al reconectar', error)
})

client.on('error', (error) => {
    console.log('Error de conexión:', error)
})
</script>

</script>
