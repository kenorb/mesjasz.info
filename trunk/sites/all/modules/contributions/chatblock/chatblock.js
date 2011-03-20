// $Id$

// global variable to keep track of responses from the server
var _chatblock_response_timestamps = new Array();

/**
 * This function scrolls the div to the bottom, buggy in Opera so we add a <br /> to the bottom of our div
 */
function chatboxInitScroll() {
  var objDiv = document.getElementById("chatblock_block");
  // all but Explorer Mac
  if (objDiv.scrollHeight > 0) {
    objDiv.scrollTop = 0;
    objDiv.scrollTop = objDiv.scrollHeight - objDiv.offsetHeight;
  }
  // Explorer Mac;
  //would also work in Explorer 6 Strict, Mozilla and Safari
  else {
    objDiv.scrollTop = objDiv.offsetHeight;
  }
}

/**
 * This function checks for new messages at the rate specified by the user 
 */
function chatboxGetMessage() {
  // we could use a jQuery selector here, but why bother?
  var timestamp = document.getElementById("chatblocklastmessage").innerHTML;

  // create the message to send to the server, including a random token so IE won't cache the response
  var message = "&timestamp=" + timestamp + "&chatboxtoken=" + Math.random(); 
  
  // send the data to the server
  $.ajax({ type: "POST", url: chatBlockViewURL, data: message, success: function(text) { chatblockResponse(text); } });
}

/**
 * This function handles the json response from the server
 * We were using an html response, but occasionally we receive duplicate responses
 * so we need to be able to keep track of the timestamps, which is easier using json
 */
function chatblockResponse(jsonStringResponse) {
  // if we have received a response
  if (jsonStringResponse != "") {
    // try and evaluate the response
    try {
      // can we parse the response?
      var messages = eval("(" + jsonStringResponse + ")");
    }
    catch(e) {
      // if not, use an empty array
      var messages = new Array();
    }
    // if we have any message responses, check to make sure we haven't seen them already
    for (var timestamp in messages) {
      if (timestamp == 0) {
        // update the last timestamp
        // we have to remove the m added to provide Opera support because of a bug in how Opera handles json Response text with large integers
        document.getElementById("chatblocklastmessage").innerHTML = messages[timestamp].replace('m', '');
      }
      else {
        if (_chatblock_response_timestamps.indexOf(timestamp) > -1 ) {
          // do nothing, this is a message that has been added already
        }
        else {
          // add the message to the display window
          $("#chatblock_block").children("br").before('<div class="chatblock-message"><span class="chatblock-username">' + messages[timestamp]['username'] + ': </span> ' + messages[timestamp]['message'] + '</div>');
          // keep track of the current messages
          _chatblock_response_timestamps.push(timestamp);
        }
      }
    }
    // if we have any messages, make sure to scroll to the bottom of the div
    chatboxInitScroll();
  }
  if (document.getElementById("edit-chatblocksubmit")) {
    document.getElementById("edit-chatblocksubmit").disabled = false;
  }
  return;
}

/**
 * This function is called when the user wants to send a message to the database
 * It expects the server to response with any new messages (including the users message, filtered)
 */
function chatblockSend(el) {
  // disable the submit button so the user can't submit twice
  el.disabled = true;
  
  // get the current timestamp
  // we could use a jQuery selector here, but why bother?
  var timestamp = document.getElementById("chatblocklastmessage").innerHTML;
  
  // build the message to send to the database, make sure to use a random token to prevent IE from caching the response
  var message = "&timestamp=" + timestamp + "&chatboxtoken=" + Math.random() + "&message=" + document.getElementById("edit-chatblocktext").value;
  
  // empty out the value of the textfield the user sent his message from
  document.getElementById("edit-chatblocktext").value = "";
  
  // send the message
  $.ajax({ type: "POST", url: chatBlockUpdateURL, data: message, success: function(text) { chatblockResponse(text); }, error: function (XMLHttpRequest, textStatus, errorThrown) { console.error(XMLHttpRequest.status); } });
  
  // don't submit the form regularly
  return false;
}

/**
 * IE sucks - need to add indexOf method for arrays
 */
if(!Array.indexOf){
  Array.prototype.indexOf = function(obj){
    for(var i=0; i<this.length; i++){
      if(this[i]==obj) {
        return i;
      }
    }
    return -1;
  }
}
