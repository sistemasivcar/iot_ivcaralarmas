<div ui-view class="app-body" id="view">

<!-- ############ PAGE START-->

  <div class="item">
    <div class="item-bg">
      <img src="assets/images/logo.png" class="blur opacity-3">
    </div>
    <div class="p-a-md">
      <div class="row m-t">
        <div class="col-sm-7">
          <a href class="pull-left m-r-md">
            <span class="avatar w-96">
              <img src="assets/images/imguser.png">
              <i class="on b-white"></i>
            </span>
          </a>
          <div class="clear m-b">
            <h3 class="m-0 m-b-xs"><?php echo $name; ?></h3>


            <br><br>
          </div>
        </div>

      </div>
    </div>
  </div>
  
  <div class="padding">
    <div class="row">
      <div class="col-sm-8 col-lg-9">
        <div class="tab-content">
          <div class="tab-pane p-v-sm active" id="tab_1">
            <div class="tab-pane p-v-sm" id="tab_4">
              <div class="row m-b">
                <div class="col-xs-6">
                  <small class="text-muted">Nombre de Usuario</small>
                  <div class="_500"><?php echo $name ?></div>
                </div>
                <div class="col-xs-6">
                  <small class="text-muted">Teléfono</small>
                  <div class="_500"><?php echo $phone ?></div>
                </div>
              </div>
              <div class="row m-b">
                <div class="col-xs-6">
                  <small class="text-muted">Correo Electrónico</small>
                  <div class="_500"><?php echo $email ?></div>
                </div>
                <div class="col-xs-6">
                  <small class="text-muted">Cantidad de Dispositivos</small>
                  <div class="_500"><?php   echo $count?></div>
                </div>
              </div>
              <div>
                <small class="text-muted">Fecha Inscripción</small>
                <div><?php echo $date ?></div>
              </div>
            </div>



          </div>





        </div>
      </div>
      <div class="col-sm-4 col-lg-3">

      </div>
    </div>
  </div>

<!-- ############ PAGE END-->

    </div>
  </div>
  <!-- / -
