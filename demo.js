function checkForm() {
    if(document.forms["robotForm"]["Port"].value==localStorage.getItem("Port"))
    {
	document.forms["robotForm"]["Port"].value="";
    }
    if(document.forms["robotForm"]["Terminate"].value=="on")
    {
	localStorage.removeItem("Port");
	localStorage.removeItem("Motor");
    }
    else
	window.alert(document.forms["robotForm"]["Terminate"].value);
}

function initializeForm() {
    document.forms["robotForm"]["Port"].value=localStorage.getItem("Port")||"";
    document.forms["robotForm"]["Motor"].value=parseInt(localStorage.getItem("Motor"),10).toString(16)||"";
}
