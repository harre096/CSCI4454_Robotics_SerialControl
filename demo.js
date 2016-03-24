function changeMotorSpeed(newVal){
		//set value in hidden form (hacky)
		$("#speedInput").val(newVal);
		//run ajax command
		var form = $('#changeSpeedForm')
		$.ajax( {
				type: "POST",
				url: form.attr( 'action' ),
				data: form.serialize(),
				success: function( response ) {
				  console.log( response );
				}
			});
}

function changeMotorSpeedManual(){
	changeMotorSpeed($("#speedInputShell").val())
	console.log($("#speedInputShell").val())
}

$(document).ready( function() {

//submit motor change
var form = $('#changeSpeed');

$("#changeMotorSpeed").click(function() {
  
});

//makes it so pressing enter on "clicks" submit
$('#speedInput').keypress(function (e) {
 var key = e.which;
 if(key == 13)  // the enter key code
	{
		$('#changeMotorSpeed').click();
		return false;  
	}
});   

});


