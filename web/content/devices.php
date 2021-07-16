
<div ui-view class="app-body" id="view">


        <!-- SECCION CENTRAL -->
        <div class="padding">

          <div class="row">
            <div class="col-md-6">
              <div class="box">
                <div class="box-header">


                  <h2>Agregar Dispositvos</h2>
                  <small>Ingresa el nombre (alias) y el número de serie del dispositivo que quieres instalar.</small>

                  <?php if($msj_add == "Ese número de serie no está disponible"){
                  echo '<div style="color:red">';} else{ echo '<div style="color:lime">';}
                  echo $msj_add;?>
                  </div>
                </div>
                <div class="box-divider m-0"></div>
                <div class="box-body">


                  <form role="form" method="post" target="">
                    <div class="form-group">
                      <label for="exampleInputEmail1">Alias</label>
                      <input name="alias_to_add" type="text" class="form-control" value="<?php echo $alias_to_add; ?>" placeholder="Ej: Casa Campo">
                    </div>
                    <div class="form-group">
                      <label for="exampleInputPassword1">Serie</label>
                      <input name="serie_to_add" type="number" class="form-control" placeholder="Ej: 777222">
                    </div>

                    <button type="submit" class="btn green m-b">Registrar</button>

                  </form>


                </div>
              </div>
            </div>

          </div>

          <div class="row">
            <div class="col-sm-7">
              <div class="box">
                <div class="box-header">
                  <h2>Mis dispositivos</h2>
                  <?php if($msj_change == "Ese número de serie no está disponible"){
                  echo '<div style="color:red">';} else{ echo '<div style="color:lime">';}
                  echo $msj_change;?>
                  </div>
                </div>
                <table id="data_table" class="table table-striped b-t">
                  <thead>
                    <tr>
                      <th>ID</th>
                      <th>Alias</th>
                      <th>Serie</th>
                      <th>Estado</th>
                    </tr>
                  </thead>
                  <tbody>
                    <?php foreach ($devices as $device) {?>
                      <tr id="<?php echo $device['devices_id']; ?>">
                        <td><?php echo $device['devices_id'] ?></td>
                        <td><?php echo $device['devices_alias'] ?></td>
                        <td><?php echo $device['devices_serie'] ?></td>
                        <td> <?php if ($device['devices_status'] == 0){ echo "<div style='color:red'><i class='fa fa-times'></div>";} else if  ($device['devices_status'] == 1) { echo "<div style='color:lime'><i class='fa fa-check-circle'></div>"; }else{echo "nada";} ?></td>

                        <td><?php echo "<button type='button' class='btn danger m-b' ><a href='devices.php?id_to_delete=".$device['devices_id'] ."'><i class='fa fa-trash'></i></button></a>"?></td>

                      </tr>
                    <?php } ?>
    </tbody>
  </table>
  </div>
  </div>
  </div>


  </div>




  </div>
