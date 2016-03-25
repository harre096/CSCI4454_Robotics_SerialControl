function setPort(newVal){
		//set value in hidden form (hacky)
		$("#portInput").val(newVal);
		//run ajax command
		var form = $('#genericForm')
		$.ajax( {
				type: "POST",
				url: form.attr( 'action' ),
				data: form.serialize(),
				success: function( res ) {
				  console.log( "Defaulted to port 0." );
				}
			});
};


function setMotorSpeed(newVal){
		//set value in hidden form (hacky)
		$("#speedInput").val(newVal);
		//run ajax command
		var form = $('#genericForm')
		$.ajax( {
				type: "POST",
				url: form.attr( 'action' ),
				data: form.serialize(),
				success: function( res ) {
					$("#motorDisp").text( res );
				  console.log( res );
				}
			});
}

function closePortServer(){
		//set value in hidden form (hacky)
		$("#terminateInput").val("1");
		//run ajax command
		var form = $('#genericForm')
		$.ajax( {
				type: "POST",
				url: form.attr( 'action' ),
				data: form.serialize(),
				success: function( res ) {
					console.log( "portClosed" );
				}
			});
}


function setMotorSpeedManual(){
	setMotorSpeed($("#speedInputShell").val())
	console.log($("#speedInputShell").val())
}
function setPortManual(){
	setPort($("#portInput").val())
	console.log($("#speedInputShell").val())
}


$(document).ready( function() {

//init stuff
setPort("0");


//make change motor button work
$("#changeMotorSpeed").click(function() {
  changeMotorSpeedManual();
});

//makes it so pressing enter on "clicks" submit
$('#speedInputShell').keypress(function (e) {
 var key = e.which;
 if(key == 13)  // the enter key code
	{
		changeMotorSpeedManual();
		return false;  
	}
});   



});//close jquery



