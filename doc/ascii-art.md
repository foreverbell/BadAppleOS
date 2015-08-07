# ASCII Art

## Plain Text

The plain text is located at `video.txt` in root directory.

There are 1686 frames in total, and each frame is two-valued, with size of 80*25, fitting the default VGA mode 3.

## Zipping

The size of original plain text is about 1686\*80\*25=3,372,000 bytes, or 3.22 mega-bytes [^1], to fit in the capacity of a floppy disk, which is 1.44 mega-bytes, we have to zip it in some way. But I don't want to introduce some complicate algorithm to do that thing, so I use a quite simple way in a python script, see `kernel/encode.py`. 

[^1]: We omit `CRLF` in the plain txt file.

The algorithm is, since the frame is already two-valued, `*` for a black pixel, space for a white pixel, we just simply encode it in each bit, which will produce a binary of $\frac{1686*80*25}{8}$=421,500 bytes, or 411.6 kilo-bytes. It would be sufficient to be put in a floppy disk.

## Further Compression

Open the zipped binary produced by last step with hex editor, we can find that there are so many `0xFF` and `0x00`. If you are familiar with [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding), you may realize that we can get further compression with Huffman encoding compression algorithm, which reduces the output to only 126 kilo-bytes.

## Linking to Kernel

But how do our kernel know where the video data is?

A classical solution is to *compile* the video binary to an object file. Assuming the video binary is `vdata.bin`, we can use the following command to compile it, 
	
	objcopy -B i386 -I binary -O elf32-i386 vdata.bin vdata.o
	
Executing command `objdump -t vdata.o`, we get the export symbols, 
	
	vdata.o:     file format elf32-i386
	
	SYMBOL TABLE:
	00000000 l    d  .data	00000000 .data
	00000000 g       .data	00000000 _binary_vdata_bin_start
	00066e7c g       .data	00000000 _binary_vdata_bin_end
	00066e7c g       *ABS*	00000000 _binary_vdata_bin_size

Thus, the last step is simply to write down the belowing code to get direct access to the zipped video data.

``` {.C}
extern uint8_t vdatas[] asm("_binary_vdata_bin_start");
extern uint8_t vdatae[] asm("_binary_vdata_bin_end");
``` 

## Artifying Data

Decoding the zipped data to plain text is so easy that we omit it.

The main problem is, the plain text is too ugly to be displayed on the screen, we need to *artify* it to a better ASCII art.

A typical ASCII art uses a variety of characters to give people back the feeling of original picture. I think the reason for lacking of a great visual effect of our video is that we only use one character `*`.

Unfortunately, if we use more characters in our plain text, our zipping algorithm will lose its power. To fix this, I come up with two methods described below. 

* To avoid monotonous, we randomly change each connected block to one *dense* character, picked from the charSet `NH#*&`. Detecting the block is a quite classical problem, a [Flood Fill algorithm](https://en.wikipedia.org/wiki/Flood_fill) or a [Disjoint-set data structure](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) is suitable. And for consistency, the same block in two consecutive frames will remain the same character. [^2] However, it turns out that there will be one block keeping the same character for a long time, so each reasonably small block will have a small possibility to do a mutation avoiding this. 

[^2]: If there is conflict when merging two separate blocks, randomly pick one.

* To emphasize the boundary, we do a simple boundary check, just examining if one of the adjacent pixels is empty, then replace the boundary pixel with the correct one from ``'`.,``. And if one of the horizontal or vertical direction is both non-empty, then replace it with the line characters from `\/^` and `chr(28)` [^3].

[^3]: Display it on a terminal via a script to see what is it!

After the artifying process, our video now meets my expectation (hope it meets yours as well). Through it is still far away from [the state of art](https://www.youtube.com/watch?v=rFEc3f8TDFg) in some ways, but it does't matter, yeah? :)