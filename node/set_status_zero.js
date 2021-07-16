var mysql = require('mysql');

//CREDENCIALES MYSQL
var con = mysql.createConnection({
    host: "ivcaralarmas.com",
    user: "admin_alarmaiot",
    password: "Gaston2001",
    database: "admin_alarmaiot"
  });

  con.connect(function(err){
    if (err) throw err;

    //una vez conectados, podemos hacer consultas.
    console.log("Conexión a MYSQL exitosa!!!")

    //hacemos la consulta
    var query = "UPDATE `admin_alarmaiot`.`devices` SET `devices_status`= '0' WHERE 1";
    con.query(query, function (err, result, fields) {
      if (err) throw err;

    });

    var query = "UPDATE `admin_alarmaiot`.`last_data` SET `last_data_status`= '0' WHERE 1";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
    });

  });
