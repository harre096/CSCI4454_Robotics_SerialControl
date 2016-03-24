function setDefaultPort(){
		//set value in hidden form (hacky)
		$("#portInput").val("0");
		//run ajax command
		var form = $('#comPortForm')
		$.ajax( {
				type: "POST",
				url: form.attr( 'action' ),
				data: form.serialize(),
				success: function( res ) {
				  console.log( "Defaulted to port 0." );
				}
			});
};


function changeMotorSpeed(newVal){
		//set value in hidden form (hacky)
		$("#speedInput").val(newVal);
		//run ajax command
		var form = $('#changeSpeedForm')
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


function changeMotorSpeedManual(){
	changeMotorSpeed($("#speedInputShell").val())
	console.log($("#speedInputShell").val())
}

$(document).ready( function() {

//init stuff
setDefaultPort();
$('#comPortDisp').text(localStorage.getItem("Port")||"");



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



