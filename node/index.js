//        topico                    mensaje
//  ----------------------------------------
//    alarma/values/device  ---->   1,1,1,1
//    alarma/command/device ---->   activar

//const client_twilio = require('twilio')('ACc4088bddd2de96befccaa8d3f9e17928', 'e148cf6c08b4417e05358bf028fbc310');
var mysql = require('mysql');
var mqtt = require('.mqtt.DELETE');
// Banderas
var flag_robo_user;
var flag_res_user;
var flag_act_user;
var flag_desact_user;

var last_data_estado;
var sirena_user;

var msj_to_send; // sirena
var msg_to_send; // act/desact

//CREDENCIALES MYSQL
var con = mysql.createConnection({
  host: "ivcaralarmas.com",
  user: "admin_alarmaiot",
  password: "Gaston2001",
  database: "admin_alarmaiot"
});

//CREDENCIALES MQTT
var options = {
  port: 1883,
  host: 'ivcaralarmas.com',
  clientId: 'alarma_iot_server' + Math.round(Math.random() * (0- 10000) * -1) ,
  username: 'web_client',
  password: '0000',
  keepalive: 60,
  reconnectPeriod: 1000,
  protocolId: 'MQIsdp',
  protocolVersion: 3,
  clean: true,
  encoding: 'utf8'
};

var client = mqtt.connect("mqtt://ivcaralarmas.com", options);

// ************************
//      CONEXION MQTT
// ************************
client.on('connect', function () {
client.subscribe('alarma/#', function (err) {
  });
})

// **********************************
//      CUANDO RECIBO UN MENSAJE...
// **********************************
client.on('message', function (topic, message) {
  var topic_splitted = topic.split("/");
  var query = topic_splitted[1];
  var device_serie = topic_splitted[2];


  // ********************************************************************************************************
  //                                                    TOPICO VALUES
  // ********************************************************************************************************

  if (query == "values"){

    var msg = message.toString();
    var sp = msg.split(",");

    var estado = sp[0];
    var sirena = sp[1];
    var mov = sp[2];
    var abertura = sp[3];


    // SI EL DISPOSITIVO LE PERTENECE A ALGÚN USUARIO (sino no manda sms)
    var query = "SELECT * FROM devices WHERE devices_serie = " + device_serie + "";
    con.query(query, function (err, result, fields) {
    if (err) throw err;

    if (result.length > 0){

    // CONSULTO EL ESTADO DE CENTRAL MONITOREO
    var query = "SELECT status_monitoreo FROM last_data WHERE last_data_serie = 2556800";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
    status_monitoreo = result[0].status_monitoreo;



    // CONSULTO EL PHONE y NAME DE ESE USUARIO
    var query = "SELECT * FROM user_device WHERE devices_serie = " + device_serie + "";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
      phone = result[0].users_phone;
      name = result[0].users_name;
      
            // ***********************************************
            // ENVIO MENSAJE DE ACTIVACION POR CONTROL REMOTO
            // ***********************************************
          var query = "SELECT * FROM last_data WHERE last_data_serie = " + device_serie + "";
          con.query(query, function (err, result, fields) {
            if (err) throw err;
            flag_act_user = result[0].flag_act;
            flag_desact_user = result[0].flag_desact;
            last_data_estado = result[0].last_data_estado;

            if (last_data_estado == 1 && flag_act_user == 0 && status_monitoreo == 0){
              phone_to_send = "+54" + phone;
              msj_activacion = name + "," + phone_to_send + "," + device_serie

              client.publish("sms/act", name + "," + phone_to_send + "," + device_serie); // sms/activacion - msg_to_send, phone_to_send
              console.log("Publico el mensaje: " + msj_activacion);

              var query = "INSERT INTO `act_desact` (`act_desact_command`, `act_desact_device_serie`) VALUES (1, " + device_serie + ");";
              con.query(query, function (err, result, fields) {
                if (err) throw err;
              });
                // PONGO FLAG ACT EN 1
                var query = "UPDATE `admin_alarmaiot`.`last_data` SET `flag_act`= 1, `flag_desact`=0 WHERE last_data_serie = " + device_serie + "";
                con.query(query, function (err, result, fields) {
                  if (err) throw err;
                  console.log(phone_to_send)
                });

            }
            else if (last_data_estado == 0 && flag_desact_user == 0 && status_monitoreo == 0){
              phone_to_send = "+54" + phone;
              msj_desactivacion = name + "," + phone_to_send  + "," + device_serie

              client.publish("sms/desact", name + "," + phone_to_send + "," + device_serie); // sms/activacion - msg_to_send, phone_to_send
              console.log("Publico el mensaje: " + msj_desactivacion);

              var query = "INSERT INTO `act_desact` (`act_desact_command`, `act_desact_device_serie`) VALUES (0, " + device_serie + ");";
              con.query(query, function (err, result, fields) {
                if (err) throw err;
              });

                // PONGO FLAG DESACT EN 1
                var query = "UPDATE `admin_alarmaiot`.`last_data` SET `flag_act`= 0, `flag_desact`=1 WHERE last_data_serie = " + device_serie + "";
                con.query(query, function (err, result, fields) {
                  if (err) throw err;
                  console.log(phone_to_send)
                });
            }


            });

            // ****************************************
            // ENVIO DE MENSAJE DE ROBO Y RESTAURACION
            // ****************************************
            var query = "SELECT * FROM last_data WHERE last_data_serie = " + device_serie + "";
            con.query(query, function (err, result, fields) {
              if (err) throw err;
              sirena_user = result[0].last_data_sirena;
              flag_robo_user = result[0].flag_robo;
              flag_res_user = result[0].flag_res;

            if (sirena_user == 1 && flag_robo_user == 0 && status_monitoreo == 0){

              // ENVIO EL MENSAJE
              phone_to_send = "+54" + phone;
              msj_robo = name + "," + phone_to_send  + "," + device_serie

              client.publish("sms/robo", name + "," + phone_to_send + "," + device_serie); // sms/activacion - msg_to_send, phone_to_send
              console.log("Publico el mensaje: " + msj_robo);

              var query = "INSERT INTO `robo_res` (`robo_res_command`, `robo_res_device_serie`) VALUES (1, " + device_serie + ");";
              con.query(query, function (err, result, fields) {
                if (err) throw err;
              });

                // PONGO ROBO EN 1 -- RES EN 0
                var query = "UPDATE `admin_alarmaiot`.`last_data` SET `flag_robo`= 1, `flag_res`=0 WHERE last_data_serie = " + device_serie + "";
                con.query(query, function (err, result, fields) {
                  if (err) throw err;
                  console.log(phone_to_send)
                });

            }

            else if (sirena_user == 0 && flag_res_user == 0 && status_monitoreo == 0){

              // ENVIO EL MENSAJE
              phone_to_send = "+54" + phone;
              msj_res = name + "," + phone_to_send  + "," + device_serie

              client.publish("sms/res", name + "," + phone_to_send + "," + device_serie); // sms/activacion - msg_to_send, phone_to_send
              console.log("Publico el mensaje: " + msj_res);

              var query = "INSERT INTO `robo_res` (`robo_res_command`, `robo_res_device_serie`) VALUES (0, " + device_serie + ");";
              con.query(query, function (err, result, fields) {
                if (err) throw err;
              });

              // PONGO ROBO EN 0 -- RES EN 1
              var query = "UPDATE `admin_alarmaiot`.`last_data` SET `flag_robo`= 0, `flag_res`=1 WHERE last_data_serie = " + device_serie + " ";
              con.query(query, function (err, result, fields) {
                if (err) throw err;
                console.log(phone_to_send)
              });
            }

            });
          });
        });
      }
});

    // **********************************************
    // INSERTO EN DATA - VIVOS - UPDATE EN LAST_DATA
    // **********************************************

    //// INSERTO EN TABLA DATOS ////
    var query = "INSERT INTO `admin_alarmaiot`.`data` (`data_estado`, `data_sirena`, `data_mov`, `data_abertura`, `data_device_serie`) VALUES (" + estado + ", " + sirena + ", " + mov + ", " + abertura + ", " + device_serie + " );";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
      //console.log("DATO INSERTADO")
    });

    //// ********* EXISTE ESA SERIE EN TABLA LAST_DATA? ********* ////
    var query = "SELECT last_data_serie FROM last_data WHERE last_data_serie = " + device_serie + "";
    con.query(query, function (err, result, fields) {
      if (err) throw err;

      if (result.length > 0){
        // ACTUALIZO DATOS
        var query = "UPDATE `admin_alarmaiot`.`last_data` SET `last_data_estado`='" + estado + "', `last_data_sirena`='" + sirena + "', `last_data_abertura`='" + abertura + "', `last_data_mov`='" + mov + "', `last_data_status`='1' WHERE last_data_serie = " + device_serie + "";
        con.query(query, function (err, result, fields) {
          if (err) throw err;
        });
      }else{
        // INSERTO EL DATO EN LAST_DATA
        var query = "INSERT INTO `admin_alarmaiot`.`last_data` (`last_data_estado`, `last_data_sirena`, `last_data_abertura`, `last_data_mov`, `last_data_disp1`, `last_data_disp2`, `last_data_status`, `last_data_serie`) VALUES (" + estado + ", " + sirena + ", " + abertura + ", " + mov + ",0,0,1, " + device_serie + " );";
        con.query(query, function (err, result, fields) {
          if (err) throw err;
        });
      }
    });

    //// ********* PONGO EN 1 VIVOS  *********  ////
    var query = "UPDATE `admin_alarmaiot`.`devices` SET `devices_status`= '1' WHERE devices_serie = " + device_serie + "";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
    });
  }
// ********************************************************************************************************
//                                                    TOPICO COMMAND
// ********************************************************************************************************
  else if (query == "commandd"){

  var msg = message.toString();
  var phone = "";
  var phone_to_send = "";


  var query = "SELECT * FROM user_device WHERE devices_serie = " + device_serie + "";
  con.query(query, function (err, result, fields) {
    if (err) throw err;
    phone = result[0].users_phone;


    //// REGISTRO ACTIVACION ////
    if (msg == "activar"){
      var query = "INSERT INTO `act_desact` (`act_desact_command`, `act_desact_device_serie`) VALUES (1, " + device_serie + ");";
      con.query(query, function (err, result, fields) {
        if (err) throw err;
        //console.log("Comando registrado en 'TRAFFIC' ");
      });

      //// REGISTRO DESACTIVACION ////
    }else if (msg == "desactivar"){
      var query = "INSERT INTO `act_desact` (`act_desact_command`, `act_desact_device_serie`) VALUES (0, " + device_serie + ");";
      con.query(query, function (err, result, fields) {
        if (err) throw err;
        //console.log("Comando registrado en 'TRAFFIC' ");
      });

      //// REGISTRO ACT PARCIAL ////
    }else{
      var query = "INSERT INTO `act_desact` (`act_desact_command`, `act_desact_device_serie`) VALUES (2, " + device_serie + ");";
      con.query(query, function (err, result, fields) {
        if (err) throw err;
        //console.log("Comando registrado en 'TRAFFIC' ");
      });
    }

  });

  }

  // ********************************************************************************************************
  //                                                    TOPICO MONITOREO
  // ********************************************************************************************************

  else if (query == "monitoreo"){
    var msg = message.toString();
    console.log("MONITOREO DICE " + msg);

    if (msg == "0"){
      var query = "UPDATE `admin_alarmaiot`.`last_data` SET `status_monitoreo`= 0 WHERE last_data_serie = 2556800";
      con.query(query, function (err, result, fields) {
        if (err) throw err;
      });
    }else if (msg == "1"){
      var query = "UPDATE `admin_alarmaiot`.`last_data` SET `status_monitoreo`= 1 WHERE last_data_serie = 2556800";
      con.query(query, function (err, result, fields) {
        if (err) throw err;
      });
    }


  }
});//callbak


con.connect(function(err){
  if (err) throw err;
  var query = "SELECT * FROM devices WHERE 1";
  con.query(query, function (err, result, fields) {
    if (err) throw err;
  });

});
//para mantener la sesión con mysql abierta
setInterval(function () {
  var query ='SELECT 1 + 1 as result';

  con.query(query, function (err, result, fields) {
    if (err) throw err;
  });

},);
