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
    var query = "DELETE FROM data WHERE data_date < CURRENT_DATE() - INTERVAL 31 DAY";
    con.query(query, function (err, result, fields) {
      if (err) throw err;
      fail;
    });

  });
