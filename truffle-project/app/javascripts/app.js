// Import the page's CSS. Webpack will know what to do with it.
import "../stylesheets/app.css";

// Import libraries we need.
import { default as Web3} from 'web3';
import { default as contract } from 'truffle-contract'

// Import our contract artifacts and turn them into usable abstractions.
import random_artifacts from '../../build/contracts/Random.json'
import purchase_artifacts from '../../build/contracts/Purchase.json'

// Random is our usable abstraction, which we'll use through the code below.
var Random = contract(random_artifacts);
var Purchase = contract(purchase_artifacts);

// The following code is simple to show off interacting with your contracts.
// As your needs grow you will likely need to change its form and structure.
// For application bootstrapping, check out window.addEventListener below.
var accounts;
var account;
var seller;
var buyer;

var mysql      = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'qwer',
  database : 'eth'
});

window.App = {
  start: function() {
    var self = this;

    // Bootstrap the Random abstraction for Use.
    Random.setProvider(web3.currentProvider);
    Purchase.setProvider(web3.currentProvider)

    // Get the initial account balance so it can be displayed.
    web3.eth.getAccounts(function(err, accs) {
      if (err != null) {
        alert("There was an error fetching your accounts.");
        return;
      }

      if (accs.length == 0) {
        alert("Couldn't get any accounts! Make sure your Ethereum client is configured correctly.");
        return;
      }

      accounts = accs;
      account = accounts[0];
      seller = accounts[1];
      buyer = accounts[2];

    });
  },

  setFlag: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      var flag1=Math.floor(Math.random()*9);
      document.getElementById("setflag").innerHTML="setflag: "+flag1;
      return meta.setNumber(flag1,{from: account})
    }).then(function() {
      
    }).catch(function(e) {
      console.log(e);
    });
  },

  getAll: function() {
    var self = this;

    self.getBlocknumber();
    self.getBlockdifficulty();
    self.getBlockgaslimit();
    self.getBlocktimestamp();
    self.getTxgasprice();
    self.getRandom();
  },
 
  getTxgasprice: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getTxgasprice();  
    }).then(function(value) {
      document.getElementById("txgasprice").innerHTML="txgasprice: "+value;
    }).catch(function(e) {
      console.log(e);
    });
  },

  getBlocktimestamp: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getBlocktimestamp();  
    }).then(function(value) {
      document.getElementById("blocktimestamp").innerHTML="blocktimestamp: "+value;
    }).catch(function(e) {
      console.log(e);
    });
  },

  getBlockgaslimit: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getBlockgaslimit();  
    }).then(function(value) {
      document.getElementById("blockgaslimit").innerHTML="blockgaslimit: "+value;
    }).catch(function(e) {
      console.log(e);
    });
  },
  
  getBlockdifficulty: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getBlockdifficulty();  
    }).then(function(value) {
      document.getElementById("blockdifficulty").innerHTML="blockdifficulty: "+value;
    }).catch(function(e) {
      console.log(e);
    });
  },

  getBlocknumber: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getBlocknumber();  
    }).then(function(value) {
      document.getElementById("blocknumber").innerHTML="blocknumber: "+value;
    }).catch(function(e) {
      console.log(e);
    });
  },

  getRandom: function() {
    var self = this;

    var meta;
    Random.deployed().then(function(instance) {
      meta = instance;
      return meta.getNumber();  
    }).then(function(value) {
      var flag2=Math.floor(Math.random()*9);
      var final=value;
      for(var i=0;i<flag2;i++)
      {
         if(final==0){
            final=9;
         }
         else{
         final--;
         }
      }
      document.getElementById("randomnumber").innerHTML="randomnumber:  "+value+" <-- = "+flag2+" = "+final;
    }).catch(function(e) {
      console.log(e);
    });
  },

  sellGoods: function() {
    var self = this;

    var meta;
    Purchase.deployed().then(function(instance) {
      meta = instance;
      return meta.purchase(10,{from: seller})
    }).then(function() {
    }).catch(function(e) {
      alert(e);
    });
  },

  getSomething: function() {
    var self = this;

    var meta;
    Purchase.deployed().then(function(instance) {
      meta = instance;
      return meta.getSomething();
    }).then(function(value) {
      alert(value);
    }).catch(function(e) {
      alert(e);
    });
  },

  buyGoods: function() {
    var self = this;

    var meta;
    Purchase.deployed().then(function(instance) {
      meta = instance;
      return meta.confirmPurchase({from: buyer, value:web3.toWei(10,"ether")});
    }).then(function(value) {
      
    }).catch(function(e) {
      alert(e);
    });
  },

  getGoods: function() {
    var self = this;

    var meta;
    Purchase.deployed().then(function(instance) {
      meta = instance;
      return meta.confirmReceived({from: buyer});
    }).then(function(value) {
      
    }).catch(function(e) {
      alert(e);
    });
  },

  refund: function() {
    var self = this;

    var meta;
    Purchase.deployed().then(function(instance) {
      meta = instance;
      return meta.refund({from: seller});
    }).then(function(value) {
      
    }).catch(function(e) {
      alert(e);
    });
  },

  jump: function() {
    window.location.href="random.html";
  },

  login: function() {
    var username=document.getElementById("username").innerHTML;
    connection.connect();
 
    connection.query('SELECT password from seller_table where store=="ustc"', function (error, results, fields) {
      if (error) throw error;
      alert(results[0].solution);
    });
    //window.location.href="random.html";
  }
};


window.addEventListener('load', function() {
  // Checking if Web3 has been injected by the browser (Mist/MetaMask)
  if (typeof web3 !== 'undefined') {
    console.warn("Using web3 detected from external source. If you find that your accounts don't appear or you have 0 , ensure you've configured that source properly. If using MetaMask, see the following link. Feel free to delete this warning. :) http://truffleframework.com/tutorials/truffle-and-metamask")
    // Use Mist/MetaMask's provider
    window.web3 = new Web3(web3.currentProvider);
  } else {
    console.warn("No web3 detected. Falling back to http://127.0.0.1:9545. You should remove this fallback when you deploy live, as it's inherently insecure. Consider switching to Metamask for development. More info here: http://truffleframework.com/tutorials/truffle-and-metamask");
    // fallback - use your fallback strategy (local node / hosted node + in-dapp id mgmt / fail)
    window.web3 = new Web3(new Web3.providers.HttpProvider("http://127.0.0.1:7545"));
  }

  App.start();
});
