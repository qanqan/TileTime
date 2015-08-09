(function() {
	loadOptions();
	submitHandler();
})();

function submitHandler() {
	var $submitButton = $('#submitButton');

	$submitButton.on('click', function() {
		console.log('Submit button is pressed');
	
		var return_to = getQeuryParam('return_to', 'pebblejs://close#');
		document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
		document;
	});
}

function loadOptions() {
	var $language = $('#language');
	var $tijd = $('#tijd');
	var $box1 = $('#box1');
	var $big1 = $('#big1');
	var $colbox1 = $('#colbox1');
	var $txtbox1 = $('#txtbox1');
	var $box2 = $('#box2');
	var $big2 = $('#big2');
	var $colbox2 = $('#colbox2');
	var $txtbox2 = $('#txtbox2');
	var $box3 = $('#box3');
	var $big3 = $('#big3');
	var $colbox3 = $('#colbox3');
	var $txtbox3 = $('#txtbox3');
	var $box4 = $('#box4');
	var $big4 = $('#big4');
	var $colbox4 = $('#colbox4');
	var $txtbox4 = $('#txtbox4');

	if (localStorage.language) {
		$language[0].value = localStorage.language.toString();
		$tijd[0].checked = localStorage.tijd === 'true';
		$box1[0].value = localStorage.box1.toString();
		$big1[0].checked = localStorage.big1 === 'true';
		$colbox1[0].value = localStorage.colbox1;
		$txtbox1[0].value = localStorage.txtbox1;
		$box2[0].value = localStorage.box2.toString();
		$big2[0].checked = localStorage.big2 === 'true';
		$colbox2[0].value = localStorage.colbox2;
		$txtbox2[0].value = localStorage.txtbox2;
		$box3[0].value = localStorage.box3.toString();
		$big3[0].checked = localStorage.big3 === 'true';
		$colbox3[0].value = localStorage.colbox3;
		$txtbox3[0].value = localStorage.txtbox3;
		$box4[0].value = localStorage.box4.toString();
		$big4[0].checked = localStorage.big4 === 'true';
		$colbox4[0].value = localStorage.colbox4;
		$txtbox4[0].value = localStorage.txtbox4;
	}
}

function getAndStoreConfigData() {
	var $language = $('#language');
	var $tijd = $('#tijd');
	var $box1 = $('#box1');
	var $big1 = $('#big1');
	var $colbox1 = $('#colbox1');
	var $txtbox1 = $('#txtbox1');
	var $box2 = $('#box2');
	var $big2 = $('#big2');
	var $colbox2 = $('#colbox2');
	var $txtbox2 = $('#txtbox2');
	var $box3 = $('#box3');
	var $big3 = $('#big3');
	var $colbox3 = $('#colbox3');
	var $txtbox3 = $('#txtbox3');
	var $box4 = $('#box4');
	var $big4 = $('#big4');
	var $colbox4 = $('#colbox4');
	var $txtbox4 = $('#txtbox4');

	var options = {
		language: parseInt($language.val()),
		tijd: $tijd[0].checked,
		box1: parseInt($box1.val()),
		big1: $big1[0].checked,
		colbox1: $colbox1.val(),
		txtbox1: $txtbox1.val(),
		box2: parseInt($box2.val()),
		big2: $big2[0].checked,
		colbox2: $colbox2.val(),
		txtbox2: $txtbox2.val(),
		box3: parseInt($box3.val()),
		big3: $big3[0].checked,
		colbox3: $colbox3.val(),
		txtbox3: $txtbox3.val(),
		box4: parseInt($box4.val()),
		big4: $big4[0].checked,
		colbox4: $colbox4.val(),
		txtbox4: $txtbox4.val()
	};
	console.log('Language is now: ' + parseInt($language.val()));
	console.log('Tijd is now: ' + $tijd[0].checked);
	console.log('Box1 is now: ' + parseInt($box1.val()));
	console.log('Big1 is now: ' + $big1[0].checked);
	console.log('Colbox1 is now: ' + $colbox1.val());
	console.log('Txtbox1: is now: ' + $txtbox1.val());
	console.log('Box2 is now: ' + parseInt($box2.val()));
	console.log('Big2 is now: ' + $big2[0].checked);
	console.log('Colbox2 is now: ' + $colbox2.val());
	console.log('Txtbox2: is now: ' + $txtbox2.val());
	console.log('Box3 is now: ' + parseInt($box3.val()));
	console.log('Big3 is now: ' + $big3[0].checked);
	console.log('Colbox3 is now: ' + $colbox3.val());
	console.log('Txtbox3: is now: ' + $txtbox3.val());
	console.log('Box4 is now: ' + parseInt($box4.val()));
	console.log('Big4 is now: ' + $big4[0].checked);
	console.log('Colbox4 is now: ' + $colbox4.val());
	console.log('Txtbox4: is now: ' + $txtbox4.val());
	
	localStorage.language = options.language;
	localStorage.tijd = options.tijd;
	localStorage.box1 = options.box1;
	localStorage.big1 = options.big1;
	localStorage.colbox1 = options.colbox1;
	localStorage.txtbox1 = options.txtbox1;
	localStorage.box2 = options.box2;
	localStorage.big2 = options.big2;
	localStorage.colbox2 = options.colbox2;
	localStorage.txtbox2 = options.txtbox2;
	localStorage.box3 = options.box3;
	localStorage.big3 = options.big3;
	localStorage.colbox3 = options.colbox3;
	localStorage.txtbox3 = options.txtbox3;
	localStorage.box4 = options.box4;
	localStorage.big4 = options.big4;
	localStorage.colbox4 = options.colbox4;
	localStorage.txtbox4 = options.txtbox4;
	
	console.log('All values have been read: ' + JSON.stringify(options));
	return options;
}	

function getQeuryParam(key, defaultValue) {
		var query = location.search.substring(1);
		var keys = query.split('&');
		for (var i = 0; i <keys.length; i++) {
			var pair = keys[i].split('=');
			if (pair[0] === key) {
				return decodeURIComponent(pair[1]);
			}
		}
		return defaultValue || false;
	}
