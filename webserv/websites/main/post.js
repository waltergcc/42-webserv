document.querySelector('.custom-file-upload').addEventListener('click', function() {
    document.getElementById('file').click();
});


document.getElementById('file').addEventListener('change', function() {
    const fileInput = document.getElementById('file');
    const fileNameDisplay = document.getElementById('file-name');

    if (fileInput.files.length > 0) {
        fileNameDisplay.textContent = `Selected file: ${fileInput.files[0].name}`;
    } else {
        fileNameDisplay.textContent = '';
    }
});
