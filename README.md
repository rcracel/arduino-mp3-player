# MP3 Arduino Library


## History

In 2024, I decided I’d had enough of wasting money on Halloween animatronics that didn’t live up to their promises—breaking down, underperformed, or outright failing right out of the box. As an engineer, I saw this as a challenge worth tackling head-on. If the market couldn’t deliver what my daughter and I envisioned for our ultimate Halloween experience, I’d build it myself.

Armed with a 3D printer, a handful of Arduino boards, and a mountain of enthusiasm, I dove into the world of digital electronics. My goal? To make this the last Halloween where I relied on someone else’s solutions. But what started as an ambitious DIY project quickly turned into a crash course in frustration. One of the biggest roadblocks? A tiny MP3 module that seemed determined to test the limits of my patience.

Over the next three months, I found myself wrestling with everything from basic circuits to the finicky quirks of this little device. No matter how hard I tried, I couldn’t get the consistency or reliability I needed for my project. It wasn’t just a technical hurdle—it was a reminder of how many people out there must feel the same way when trying to bring their creative ideas to life.

That’s when I decided to take a different approach. Instead of fighting against the limitations of the module, I’d write my own driver—one that gave me the control and flexibility to push my Halloween ambitions further. This journey has been equal parts challenging and rewarding, and if you’ve ever felt the same frustration trying to make something truly your own, I hope what I’ve learned can make your path just a little bit easier.

## How it works

## Module initialization

At it's most basic form, this library requires only a pointer to a hardware or software serial to work properly. Invocation of ```begin()``` will attempt to send a reset to the MP3 module and wait for the module to respond back. 

### Important to know:

1. Confirm that you have configured the right RX and TX pins on your board. Remember that the board's RX needs to be connected to the DFPlayer's TX and vice versa. Also keep in mind that some implementations of SoftwareSerial require the RX pin to support interruptions. I will add more documentation once I have a chance to do more research on this topic.
2. The initialization function waits for the DFPlayer to acknowledge that it has been initialized properly. Without this acknowledgement, this library will not send any further commands to the DFPlayer. You can override this by setting ```waitUntilInitialized``` to ```false```, but I would strongly recommend against it as it can send you down a spiral of inconsistent results.
3. You can override the default timeout for the module. You can chance that by setting the ```timeout``` flag to a different value. (the default is ```2000```)
4. **DO NOT FORGET** to initialize the serial module **BEFORE** calling ```begin()```, this will cause the initialization of the MP3 module to fail silently and drive you mad.

### Method:

```uint8_t begin(Stream *stream, uint8_t busyPin, bool waitUntilInitialized = true, uint32_t timeout = 2000);```

### Example:

```
SoftwareSerial mp3Serial(RX_PIN, TX_PIN);
MP3 mp3;

void setup() {
    Serial.begin(115200);

    while (!Serial) { delay(0); }
    
    mp3.begin(&mp3Serial, 0, true);
}
```

## Looping

Since the method does not rely on interrupts or delays, you must add a call to ```loop()``` to your main loop function. 

### Important to know:

Loop serves two primary functions, in this order:

1. It checks the serial connector to see if there are new responses from the MP3 module that needs to be processed.
2. It will check the outbound message queue and send the next message if the MP3 module is ready to receive. 

### Method:

```void loop();```

### Example:

```
void loop() {
    mp3.loop();
}
```

## Sending commands

This library implements a set of the most common commands and queries used to interact with the MP3 module. 

### Important to know:

It is important to note that invoking any of these commands does not ensure when (and if) the command will be sent to the MP3 module.

### Methods:

```
  void play();
  void playTrack(uint8_t track);
  void pause();
  void stop();
  void eq(uint8_t mode);
  void dac(uint8_t onOff);
  void next();
  void prev();
  void volume(uint8_t vol);
```
### Example:

```
void loop() {
    static long lastTime = 0;
    long now = millis();
    
    // Change the track every 2 seconds
    if (now - lastTime >= 2000) {
        // Note that the track may not be changed right away, since the
        // library will wait until the module is "ready" to receive the next command
        // before attempting it
        mp3.next();
        lastTime = now;
    }
}
```

## Receiving messages

### Important to know:

### Methods:

### Example:

## Troubleshooting

If you can't get the module to initialize properly, try the following:

- Make sure your DPPlayer and your Arduino board share a **_common negative_**. Without the common 
negative, the current will not flow from the Arduino's DIO pin to the DFPlayer correctly.
- Make sure your DFPlayer is receiving enough current and voltage. During certain IO operations
the **_DFPlayer can require as much as 1A_**, and although I have heard people mention that they have
been successful with 3V input, mine has never worked properly with less than 5V.
- Make sure you understand what **_hardware & software serial_** ports are available on your board.
Most boards I have used have a single hardware serial port, and this port is used when your board
is plugged in to your computer via USB. 

## Documentation

### Class Definition

```
class MP3 {
  MP3();

  uint8_t begin(Stream *stream, bool waitUntilInitialized = true, uint32_t timeout = 2000);
  void loop();

  void reset();
  
  void onMessage(messageReceivedCallback cb);

  void play();
  void playTrack(uint8_t track);

  void pause();
  void stop();
  void eq(uint8_t mode);

  void dac(uint8_t onOff);

  void next();
  void prev();

  void volume(uint8_t vol);
};
```

### void loop()
### void reset()
### void onMessage(messageReceivedCallback cb)
### void play()
### void playTrack(uint8_t track)
### void pause()
### void stop()
### void eq(uint8_t mode)
### void dac(uint8_t onOff)
### void next();
### void prev();
### void volume(uint8_t vol);