
///////////////////////
// Custom Cup System //
///////////////////////

// Fix Z-Order of the selected cup so it doesn't go over the repick prompt
kmWrite32(0x808A81F8, 0x41F00000);
kmWrite32(0x808A8208, 0x41F00000);
