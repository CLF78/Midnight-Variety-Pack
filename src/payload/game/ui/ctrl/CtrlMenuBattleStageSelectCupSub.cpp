
///////////////////////
// Custom Cup System //
///////////////////////

// Fix Z-Order of the selected cup so it doesn't go over the repick prompt
kmWrite32(0x808A7B74, 0x41F00000);
