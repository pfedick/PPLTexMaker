# PPLib Texture generator: Format der Textur-Datei 

Eine Texture-Sprite-Datei wird zum Speichern von Sprites verwendet. Dabei werden viele einzelne kleine Sprite-Grafiken auf einer oder mehreren größeren Texturen zusammengefasst.
Die Datei verwendet als Basisformat das "PFP-File-Format Version 3" mit seinen Chunks. Dokumentation
dazu ist in pplib zu finden: pplib/docs/formate/PFPVersion3.dox (Doxygen-Format).
Die Endung der Datei ist frei wählbar, in der Regel "*.tex".

Das File trägt als ID "TEXS", Hauptversion 1, Unterversion 0. Eine Texturedatei enthält immer einen
Index-Chunk (INDX) und beliebig viele Surface-Chunks (SURF).

### INDX: Index-Chunk

Der INDX-Chunk beginnt mit einem 4-Byte-Wert im Little Endian Format, der angibt, wieviele Sprites
im Index vorhanden sind.

    Byte 0:  Anzahl Sprites                           4 Byte


Darauf folgt dann pro Sprite ein 22 Byte langer Block mit folgenden Daten:


    Byte 0:  ItemId                                   4 Byte
    Byte 4:  TextureId                                2 Byte
    Byte 6:  TextureRect                              8 Byte
            Byte 6:  left          (2 Byte)
            Byte 8:  top           (2 Byte)
            Byte 10: right         (2 Byte)
            Byte 12: bottom        (2 Byte)
    Byte 14: Pivot-Punkt                              4 Byte
            Byte 14: x-Koordinate  (2 Byte)
            Byte 16: y-Koordinate  (2 Byte)
    Byte 18: Offset                                   4 Byte
            Byte 18: x-Koordinate  (2 Byte)
            Byte 20: y-Koordinate  (2 Byte)

Sämtliche Werte müssen im Little-Endian-Format angegeben werden.

Beschreibung:
<ul>
<li><b>ItemId</b><br>
Jedes Sprite hat seine eigene ID. Diese muss nicht zwangsläufig ein fortlaufender Zähler sein, sondern
kann eine beliebige Ziffer sein. Sie muss nur eindeutig sein.
</li>
<li><b>TextureId</b><br>
Eine Sprite-Datei kann mehrere Texturen enthalten. Dieser Wert stellt eine Referenz auf die
ID der Textur dar.
</li>
<li><b>TextureRect</b><br>
Da pro Textur mehrere Sprites gespeichert werden, müssen dessen Koordinaten innerhalb der Textur
festgehalten werden. Diese geschieht innerhalb dieses Datenblocks, bei dem jeder Wert mit 2 Byte
im Little-Endian Format dargestellt wird.
</li>
<li><b>Pivot-Punkt</b><br>
Der Pivot-Punkt oder auch Ursprung ist die Position innerhalb des Sprites, aber der mit dem Zeichnen
begonnen wird. Bei einer Figur könnte dies z.B. der Mittelpunkt der Füße sein, bei einem Baum das
unterste Ende des Stamms.
</li>
<li><b>Offset</b><br>
Falls das Sprite in der Quellgrafik nicht bündig mit der ersten Pixelreihe bzw. Pixelspalte
beginnt, wird der sogenannte Offset gespeichert. Er gibt ganz einfach Spalte und Zeile an, ab der
das Sprite beginnt.
</li>
</ul>

### SURF: Surface-Chunk
Der Surface-Chunk setzt sich aus einem 8 Byte langem Header, gefolgt von den Zlib-komprimierten
Pixel-Daten zusammen:

    Byte 0:  TextureId                                2 Byte
    Byte 2:  RGB-Format                               1 Byte
    Byte 3:  Bittiefe                                 1 Byte
    Byte 4:  Breite                                   2 Byte
    Byte 6:  Höhe                                     2 Byte

Beschreibung:
<ul>
<li><b>TextureId</b><br>
Eindeutige ID der Textur, in der Regel eine fortlaufende Nummer. Wird als Referenz im INDX-Chunk
verwendet.
</li>
<li><b>RGB-Format</b><br>
Eine eindeutige Nummer, die das Farbformat der Grafikdaten angibt. Folgende Formate sind definiert:
- 9: A8R8G8B8
</li>
<li><b>Bittiefe</b><br>
Die Bittiefe eines einzelnen Farbwertes. Ist abhängig vom RGB-Format und in der Regel 32 Bit.
</li>
<li><b>Breite/Höhe</b><br>
Gibt Breite und Höhe der Grafik in Pixel an. Texturen unterliegen gewissen Einschränkungen, daher
sollten Höhe und Breite möglichst identisch und ein Vielfaches von 2 sein, also beispielsweise 32, 64,
128, 256, 512, 1024, 2048. Die meisten Grafikkarten unterstützen Texturen bis zu einer Größe von 4096 x 4096
Pixel.
</li>
</ul>

Die Pixel-Daten werden mit der Funktion ppl6::CCompression::CompressZlib komprimiert. Sie beginnen daher
mit einem 9-Byte langen Header, gefolgt von den eigentlichen Zlib-komprimierten Daten:

    Byte 0:  Komprimierunsart, ist immer 1 für Zlib       1 Byte
    Byte 1:  Anzahl Bytes unkomprimiert (litte endian)    4 Byte
    Byte 5:  Anzahl Bytes komprimiert (litte endian)      4 Byte
    Byte 9:  Beginn des komprimierten Datenstroms



### NORM: Normal-Map-Chunk (optional)
Ist genauso aufgebaut, wie der SURF-Chunk, enthält jedoch anstelle der Farbwerte
die Normalenvektoren für jedes Pixel. Diese werden benötigt, wenn
Licht- und Schatteneffekte auf die Sprites angewendet werden sollen.

### SPEC: Specular-Map-Chunk (optional)
Ist genauso aufgebaut, wie der SURF-Chunk, enthält jedoch anstelle der Farbwerte
die Specular-Werte für jedes Pixel. Diese werden benötigt, wenn
Licht- und Schatteneffekte auf die Sprites angewendet werden sollen.