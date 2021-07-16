<?php
$user_id = $_SESSION['user_id'];

$resultAlias = $conn->query(" SELECT devices_alias FROM `admin_alarmaiot`.`devices` WHERE devices_user_id = '".$user_id."' ");
$alias1= mysqli_fetch_array($resultAlias);

// SERIE DEL DISPOSITOVO
$resultSerie = $conn->query("SELECT devices_serie FROM `admin_alarmaiot`.`devices` WHERE devices_user_id = '".$user_id."' ");
$serie1= mysqli_fetch_array($resultSerie);// objeto
$serieConsulta = $serie1['devices_serie'];

// TRAIGO TODOS ROBOS
$resultRobos = $conn->query(" SELECT robo_res_command FROM `admin_alarmaiot`.`robo_res` WHERE robo_res_command = '1' AND robo_res_device_serie = '".$serieConsulta."' ");
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
<div class="col-sm-12 col-lg-12">
<div class="tab-content">
<div class="tab-pane p-v-sm active" id="tab_1">

          <!-- VALORES EN TIEMPO REAL -->
          <div class="row">
            <!-- SWItCH1 -->
            <div class="col-xs-6 col-sm-12">
              <div class="box p-a" align="center">

                <div class="clear">
                  <h4 class="m-0 text-lg _300"><input  id="img_comando_alarma_id" onclick="act_desact()" width="100" height="100"type="image" name="image" src="assets/images/loading.png"><br><small><?php echo strtoupper($alias1['devices_alias']); ?>: </small><strong><b id="estado_alarma_id" class="text-bg">--</b></strong></h4>
                  <small class="text-muted">Presione el candano para <b id ="txt_act_desact_id">--</b></small>
                </div>
              </div>
            </div>
          </div>

          <div class="row">
            <div class="col-xs-12 col-sm-4">
              <div class="box p-a" align="center">
                <div class="pull-left m-r">

                </div>
                <div class="clear">
                  <h4 class="m-0 text-lg _300"><img id="img_sirena_alarma_id" width="150" height="100" src="assets/images/loading.png"></img><span class="text-sm"></span></h4>
                  <small class="text-muted">Sirenas: <b id="txt_sirenas_id">...</b></small>
                </div>
              </div>
            </div>
            <div class="col-xs-6 col-sm-4">
              <div class="box p-a" align="center">

                <div class="clear">
                  <h4 class="m-0 text-lg _300"><img id="img_mov_alarma_id" width="150" height="100" src="assets/images/loading.png"></img><span class="text-sm"> </span></h4>
                  <small class="text-muted">Movimiento: <b id="txt_mov_id">...</b></small>
                </div>
              </div>
            </div>
            <div class="col-xs-6 col-sm-4">
              <div class="box p-a" align="center">

                <div class="clear" >
                  <h4 class="m-0 text-lg _300"><img id="img_abertura_alarma_id" width="150" height="100" src="assets/images/loading.png"></img><span class="text-sm"> </span></h4>
                  <small class="text-muted">Aberturas: <b id="txt_abertura_id">...</b></small>
                </div>
              </div>
            </div>
          </div>

          <!-- SWItCH1 y 2 -->

            <!-- SWItCH2 -->

            <h2>Comandos</h2>

            <div class="row">
              <div class="col-xs-6 col-sm-4">
                <div class="box p-a">

                  <div class="clear">
                  <input onclick="act_parcial()" width="100" height="100"type="image" name="image" src="assets/images/parcial.png" alt="50">
                    <small><b class="text-muted">Activar Parcial</b></small>
                  </div>
                </div>
              </div>
              </div>

          </div>
        </div>

        <!-- ############ PAGE END-->

      </div>

    </div>

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
        $("#img_comando_alarma_id").attr("src","assets/images/activada.png");
        $("#img_btn_comand_alarma_id").attr("src","assets/images/desactivar.png");
        $("#img_estado_alarma_id").attr("src","assets/images/activada.png");
      }else if (estado == 0){
        estado = 'DESACTIVADA'
        command = 'Activar';
        $("#img_comando_alarma_id").attr("src","assets/images/desactivad.png");
        $("#img_btn_comand_alarma_id").attr("src","assets/images/activar.png");
        $("#img_estado_alarma_id").attr("src","assets/images/desactivad.png");
      }else if (estado == 2){
        estado = 'ACTIVADA PARCIAL';
        command = 'Desactivar';
        $("#estado_alarma_id").attr("src","assets/images/act_parcial.png");
        $("#img_btn_comand_alarma_id").attr("src","assets/images/desactivar.png");
        $("#img_estado_alarma_id").attr("src","assets/images/act_parciall.png");
      }


      if (sirena == 1){
        sirenas.play();
        sirena = 'disparadas';
        $("#img_sirena_alarma_id").attr("src","assets/images/sirena_disparada.png");
      }else{
        sirenas.pause();
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
      client.publish('alarma/command/<?php echo $serie1['devices_serie'];?>', 'activar', (error) => {
          console.log(error || 'Mensaje enviado activacion!!!')
        })
        act.play();
    }

    function desactivar(){
      client.publish('alarma/command/<?php echo $serie1['devices_serie'];?>', 'desactivar', (error) => {
          console.log(error || 'Mensaje enviado desac!!!')
        })
        desact.play();

    }

    function act_parcial(){
      client.publish('alarma/command/<?php echo $serie1['devices_serie'];?>', 'parcial', (error) => {
          console.log(error || 'Mensaje enviado parcial!!!')
        })

    }


    function process_msg(topic, message){

      var sp_topic = topic.split("/");
        //var type = sp_topic[0];
        var query = sp_topic[1];
        var serie = sp_topic[2];
        //var device = sp_topic[2];

      // ej: "10,11,12"
      if (query == "values"){

        if (serie == <?php echo $serie1['devices_serie']; ?>){

        var msg = message.toString();
        var sp_msj = msg.split(",");

        var estado = sp_msj[0];
        var sirena = sp_msj[1];
        var mov = sp_msj[2];
        var abertura = sp_msj[3];
        update_values_alarma(estado,sirena,mov,abertura);
}
      }
    }



</script>
