{assign var=size value=100000000}
<h4>Upload .zip files</h4>
<i>only zip files {$size} bytes or less containing .mp3 files will be processed</i>
<p>
<form enctype="multipart/form-data" action="index.php" method="POST">
<input type="hidden" name="MAX_FILE_SIZE" value="{$size}" />
Choose a file to upload: <input name="mediafile" type="file" /><br />
<p>
<input type="hidden" name="action" value="upload/save">
<input type="submit" value="Upload File" />
</form>
<h4>Files stored locally on server</h4>
<pre>
{$files|default:none}
</pre>

