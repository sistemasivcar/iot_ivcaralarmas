<?php

$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");

//declaramos variables vacias servirán también para repoblar el formulario
$name = "";
$email = "";
$phone = "";
$password = "";
$password_r = "";
$msg = "";

if( isset($_POST['email']) && isset($_POST['password']) &&
isset($_POST['password_r']) && isset($_POST['name'])
&& isset($_POST['phone']) ) {

  $name = strip_tags($_POST['name']);
  $email = strip_tags($_POST['email']);
  $phone = strip_tags($_POST['phone']);
  $password = strip_tags($_POST['password']);
  $password_r = strip_tags($_POST['password_r']);


  if ($password==$password_r){

    //aquí como todo estuvo OK, resta controlar que no exista previamente el mail ingresado en la tabla users.
    $result = $conn->query("SELECT * FROM `users` WHERE `users_email` = '".$email."' ");
    $users = $result->fetch_all(MYSQLI_ASSOC);

    //cuento cuantos elementos tiene $tabla,
    $count = count($users);

    //solo si no hay un usuario con mismo mail, procedemos a insertar fila con nuevo usuario
    if ($count == 0){
      $password = sha1($password); //encriptar clave con sha1
      //INSERT INTO `admin_alarmaiot`.`users` (`users_name`, `users_email`, `users_phone`, `users_password`) VALUES ('gaston', 'gaston@asotn.com', '23423', '23434');
      $conn->query("INSERT INTO `admin_alarmaiot`.`users` (`users_name`, `users_email`, `users_phone`, `users_password`)
      VALUES ('".$name."', '".$email."', '".$phone."', '".$password."');");
      $msg.="Registro exitoso, ingrese haciendo <strong><a href='login.php'>clic aquí</a></strong><br>";
    }else{
      $msg.="El <strong>mail</strong> ingresado ya existe <br>";
    }

  }//pass

  else{
    $msg = "Las <strong>claves<s/trong> no coinciden";
  }

}else{
  $msg = "<strong>Complete</strong> el formulario";
}

 ?>


<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <title>Ivcar Alarmas | Registro</title>
  <meta name="description" content="Admin, Dashboard, Bootstrap, Bootstrap 4, Angular, AngularJS" />
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, minimal-ui" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge">

  <!-- for ios 7 style, multi-resolution icon of 152x152 -->
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-barstyle" content="black-translucent">
  <link rel="apple-touch-icon" href="assets/images/logo.png">
  <meta name="apple-mobile-web-app-title" content="Flatkit">
  <!-- for Chrome on Android, multi-resolution icon of 196x196 -->
  <meta name="mobile-web-app-capable" content="yes">
  <link rel="shortcut icon" sizes="196x196" href="assets/images/logo.png">

  <!-- style -->
  <link rel="stylesheet" href="assets/animate.css/animate.min.css" type="text/css" />
  <link rel="stylesheet" href="assets/glyphicons/glyphicons.css" type="text/css" />
  <link rel="stylesheet" href="assets/font-awesome/css/font-awesome.min.css" type="text/css" />
  <link rel="stylesheet" href="assets/material-design-icons/material-design-icons.css" type="text/css" />

  <link rel="stylesheet" href="assets/bootstrap/dist/css/bootstrap.min.css" type="text/css" />
  <!-- build:css ../assets/styles/app.min.css -->
  <link rel="stylesheet" href="assets/styles/app.css" type="text/css" />
  <!-- endbuild -->
  <link rel="stylesheet" href="assets/styles/font.css" type="text/css" />
</head>
<body>
  <div class="app" id="app">

<!-- ############ LAYOUT START-->
<div class="center-block w-xxl w-auto-xs p-y-md">
  <div class="navbar">
    <div align="center">
    <font size="5" class="hidden-folded inline"><strong>Registro</strong></font>
   </div>


    <div class="p-a-md box-color r box-shadow-z5 text-color m-c">

      <div style="color:red",  style="background:blue" class="">
        <?php echo $msg ?>
      </div>

      <form method="post" target="register.php" name="form">
        <div class="md-form-group">
          <input name="name" class="md-input" value="<?php echo $name; ?>" required>
          <label>Nombre Completo</label>
        </div>
        <div class="md-form-group">
          <input name="email" type="email" class="md-input" value="<?php echo $email; ?>" required>
          <label>Email</label>
        </div>
        <div class="md-form-group">
          <input name="phone" type="number" class="md-input" value="<?php echo $phone; ?>" required>
          <label>Teléfono</label>
        </div>
        <div class="md-form-group">
          <input name="password" type="password" class="md-input" required>
          <label>Password</label>
        </div>
        <div class="md-form-group">
          <input name="password_r" type="password" class="md-input" required>
          <label>Repeat Password</label>
        </div>
        <button type="submit" class="btn blue btn-block p-x-md">Crear cuenta</button>
      </form>
    </div>


<br>
    <div class="p-v-lg text-center">
      <div>Ya tenes cuenta? <a ui-sref="access.signin" href="login.php" class="text-blue _600">Ingresar</a></div>
    </div>
  </div>

<!-- ############ LAYOUT END-->

  </div>
<!-- build:js scripts/app.html.js -->
<!-- jQuery -->
  <script src="libs/jquery/jquery/dist/jquery.js"></script>
<!-- Bootstrap -->
  <script src="libs/jquery/tether/dist/js/tether.min.js"></script>
  <script src="libs/jquery/bootstrap/dist/js/bootstrap.js"></script>
<!-- core -->
  <script src="libs/jquery/underscore/underscore-min.js"></script>
  <script src="libs/jquery/jQuery-Storage-API/jquery.storageapi.min.js"></script>
  <script src="libs/jquery/PACE/pace.min.js"></script>

  <script src="html/scripts/config.lazyload.js"></script>

  <script src="html/scripts/palette.js"></script>
  <script src="html/scripts/ui-load.js"></script>
  <script src="html/scripts/ui-jp.js"></script>
  <script src="html/scripts/ui-include.js"></script>
  <script src="html/scripts/ui-device.js"></script>
  <script src="html/scripts/ui-form.js"></script>
  <script src="html/scripts/ui-nav.js"></script>
  <script src="html/scripts/ui-screenfull.js"></script>
  <script src="html/scripts/ui-scroll-to.js"></script>
  <script src="html/scripts/ui-toggle-class.js"></script>

  <script src="html/scripts/app.js"></script>

  <!-- ajax -->
  <script src="libs/jquery/jquery-pjax/jquery.pjax.js"></script>
  <script src="html/scripts/ajax.js"></script>
<!-- endbuild -->
</body>
</html>
