function changeLanguage(lang) {
	document.title = translation[lang]['title'];	
	var i, label, allElements = document.getElementsByTagName('*');
	for (i = 0; i < allElements.length; i++) {
    if (allElements[i].hasAttribute('multi')) {
			label = allElements[i].getAttribute('multi');
			if (allElements[i].hasAttribute('placeholder')) 
				allElements[i].setAttribute('placeholder',translation[lang][label]);
			else
				allElements[i].innerHTML = translation[lang][label];
    }
  }
}

var lang = window.navigator.userLanguage || window.navigator.language;
var l = lang.split("-");
lang = l[0];
if (!(lang in translation)) lang = 'en';
changeLanguage(lang);