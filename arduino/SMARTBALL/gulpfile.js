/* ESP8266 SPIFFS optimisation build
 * 
 * inspired by various sources including:
 * - https://tinkerman.cat/post/optimizing-files-for-spiffs-with-gulp
 * - https://www.bignerdranch.com/blog/css-sprite-management-with-gulp/
 * - https://www.stefanimhoff.de/gulp-tutorial-12-optimize-css-javascript-images-and-html/
 * - https://gulpjs.com/docs/en/getting-started/quick-start
 *
 */

//----------------------------------------------------------------------------------------
// DEPENDENCIES
//----------------------------------------------------------------------------------------
 
const { series, src, dest } = require('gulp');
const uglify = require('gulp-uglify');
const rename = require('gulp-rename');
const gzip = require('gulp-gzip');
const babel = require('gulp-babel');
const htmlmin = require('gulp-htmlmin');
const cleancss = require('gulp-clean-css');
const spritesmith = require('gulp.spritesmith');
const useref = require('gulp-useref');
const inline = require('gulp-inline');
const gulpif = require('gulp-if');
const del = require('del');

//----------------------------------------------------------------------------------------
// PARAMETERS
//----------------------------------------------------------------------------------------

var output = 'data/';
var source = 'html/';
var tmp    = 'tmp/';

//----------------------------------------------------------------------------------------
// TASKS
//----------------------------------------------------------------------------------------

function clean() {
	return del([output+'**/*']);
}

function clear() {
	return del([tmp]);
}

function sprite() {
  return src(source+'img/sprite/*.png')
		.pipe(spritesmith({
			imgName: 'img/sprite.png',
			cssName: 'css/sprite.css'
		}))
		.pipe(dest(source));
}

function files() {
	return src(source+'img/*.{svg,jpg,jpeg,png,ico,gif}')
		.pipe(gzip())
		.pipe(dest(output+'img/'));
}

function embed() {
	return src(tmp+'*.html')
		.pipe(inline({
			base: src,
			js: uglify,
			css: cleancss,
			disabledTypes: ['svg','img']
			}))
		.pipe(gzip())
		.pipe(dest(output));
}

function normal() {
	return src(tmp+'**/*')
		.pipe(gzip())
		.pipe(dest(output));
}

function html() {
	return src(source+'*.html')
		.pipe(useref())
		.pipe(gulpif('*.css', cleancss()))
		.pipe(gulpif(['*.js','!*-strict.js'], uglify()))
		.pipe(gulpif('*.html',htmlmin({
			collapseWhitespace: true,
			removeComments: true,
			aside: true,
			minifyCSS: true,
			minifyJS: true
		})))
		.pipe(dest(tmp));
}	

//----------------------------------------------------------------------------------------
// OPTIONS
//----------------------------------------------------------------------------------------

exports.sprite  = sprite;
exports.inline  = series(clean,sprite,files,html,embed,clear);
exports.default = series(clean,sprite,files,html,normal,clear);