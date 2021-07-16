<?php

session_start();
$logged = $_SESSION['logged'];
$name = $_SESSION['user_name'];
$user_id = $_SESSION['user_id'];


if(!$logged){
    echo '<meta http-equiv="refresh" content="0; url=login.php">';
die();
}

$show="";
$msj_add="";
$msj_change="";
$alias_to_add="";
$serie_to_add="";


$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");

// ********************
// ELIMINAR DISPOSITIVOS
// ********************

if( isset($_GET['id_to_delete']) && $_GET['id_to_delete']!="") {
    $id_to_delete = $_GET['id_to_delete'];
    $conn->query("DELETE FROM `admin_alarmaiot`.`devices` WHERE  `devices_id`=$id_to_delete");
  }

  // ********************
  // AGREGAR DISPOSITIVOS
  // ********************
  if( isset($_POST['serie_to_add']) && isset($_POST['alias_to_add']) ) {

    $alias_to_add = strip_tags($_POST['alias_to_add']);
    $serie_to_add = strip_tags($_POST['serie_to_add']);

    $alias_aux = ucfirst($alias_to_add);

    // VERIFICAR DISPONIBILIDAD DE NRO SERIE
    $result_query1 = $conn->query("SELECT * FROM `admin_alarmaiot`.`available_devices` WHERE serie = '".$serie_to_add."'");
    $device = $result_query1->fetch_all(MYSQLI_ASSOC);
    $disponibilidad = count($device);
    // VERIFICAR QUE NO LO OCUPE OTRO USUARIO
    $result_query2 = $conn->query("SELECT * FROM `admin_alarmaiot`.`devices` WHERE devices_serie = '".$serie_to_add."'");
    $devices = $result_query2->fetch_all(MYSQLI_ASSOC);
    $ocupado = count($devices);

    if($disponibilidad != 0 AND $ocupado == 0){
      $conn->query("INSERT INTO `devices` (`devices_alias`, `devices_serie`, `devices_user_id`) VALUES ('".$alias_aux."', '".$serie_to_add."', '".$user_id."');");
      $msj_add = "Dispositivo agregado";
    }else {
      $msj_add = "Ese número de serie no está disponible";
    }

  }

  // TRAIGO TODOS LOS DISPOSITOVOS
  $result = $conn->query("SELECT * FROM `devices` WHERE `devices_user_id` = '".$user_id."'");
  $devices = $result->fetch_all(MYSQLI_ASSOC);

include('application/app/Views/template/head.php');
include('application/app/Views/template/navigation.php');
include('application/app/Views/template/header.php');
include('application/app/Views/template/footer.php');


include('content/devices.php');


include('application/app/Views/template/end.php');?>
