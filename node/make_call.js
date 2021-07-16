
const accountSid = process.env.ACc4088bddd2de96befccaa8d3f9e17928;
const authToken = process.env.e148cf6c08b4417e05358bf028fbc310;
const client = require('twilio')('ACc4088bddd2de96befccaa8d3f9e17928', 'e148cf6c08b4417e05358bf028fbc310');


 client_twilio.messages
   .create({
      body: 'HOAAA',
      from: '+17028007294',
      to: '+543385448583'
    })
   .then(message => console.log(message.sid));
