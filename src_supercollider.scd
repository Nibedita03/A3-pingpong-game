// Define an L-system
(
var axiom, rules, iterations, processLSystem, lsystemResult;

// Axiom is the starting string
axiom = "A";

// Define L-system rules
rules = (
    A: "AB",
    B: "A"
);

// Function to process the L-system for a given number of iterations
processLSystem = { |axiom, rules, iterations|
    var result = axiom;
    iterations.do({
        result = result.collect { |char|
            rules[char] ?? char;
        }.join;
    });
    result
};

// Get the L-system result for a number of iterations
iterations = 5;  // You can change the number of iterations
lsystemResult = processLSystem.(axiom, rules, iterations);

// Display the L-system result
lsystemResult.postln;
)

// Define a basic SynthDef for sound generation
(
SynthDef(\simpleTone, {
    |freq = 440, dur = 0.5|
    var env = Env.perc(0.01, dur);  // Percussive envelope
    var sig = SinOsc.ar(freq) * EnvGen.kr(env, doneAction: 2);
    Out.ar(0, sig!2);  // Send stereo output
}).add;
)

// Map L-system output to sound and play it
(
var freqMap, durMap, playLSystemSound;

// Define frequency and duration mappings for A and B
freqMap = (
    A: 440,  // Frequency for A (in Hz)
    B: 660   // Frequency for B (in Hz)
);

durMap = (
    A: 0.2,  // Duration for A (in seconds)
    B: 0.4   // Duration for B (in seconds)
);

// Define a function to play sound based on the L-system result
playLSystemSound = { |lsystemResult|
    lsystemResult.do { |char|
        var freq = freqMap[char];
        var dur = durMap[char];

        // Create a simple sine wave sound
        Synth(\simpleTone, [\freq, freq, \dur, dur]);
        dur.wait;
    }
};

// Play the L-system result as sound
Routine {
    playLSystemSound.(lsystemResult);
}.play;
)
