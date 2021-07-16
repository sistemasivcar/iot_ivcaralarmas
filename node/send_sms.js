
// Download the helper library from https://www.twilio.com/docs/node/install
// Find your Account SID and Auth Token at twilio.com/console
// and set the environment variables. See http://twil.io/secure
const accountSid = process.env.ACc4088bddd2de96befccaa8d3f9e17928;
const authToken = process.env.e148cf6c08b4417e05358bf028fbc310;
const client = require('twilio')('ACc4088bddd2de96befccaa8d3f9e17928', 'e148cf6c08b4417e05358bf028fbc310');

client.messages
  .create({
     body: 'This is the ship that made the Kessel Run in fourteen parsecs?',
     from: '+15103190455',
     to: '+543385448583'
   })
  .then(message => console.log(message.sid));
