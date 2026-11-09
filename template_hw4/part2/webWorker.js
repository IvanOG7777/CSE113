// Perform computation when receive data from main.js
self.onmessage = function(event) {
    
    // Unpack data received from main.js
    var NUMPARTICLES = event.data.NUMPARTICLES;
    var start = event.data.start;
    var end = event.data.end;
    // read only particle positions
    var particlesComputeData = new Float32Array(event.data.particlesComputeBuffer);
    // writer only particle positions
    var particlesRenderData = new Float32Array(event.data.particlesRenderBuffer);

    // function to move the particles to each other
    function move_towards(self_x, self_y, nearest_x, nearest_y) {
        if (nearest_x > self_x) {
            self_x += 1;
        } else if (nearest_x < self_x) {
            self_x -= 1;
        }

        if (nearest_y > self_y) {
            self_y += 1;
        } else if (nearest_y < self_y) {
            self_y -= 1;
        }

        return [self_x, self_y];
    }


    for (let i = start; i < end; i++) {
        let self_x = particlesComputeData[2 * i];
        let self_y = particlesComputeData[2 * i + 1];

        let min_distance = Infinity;
        let nearest_x = Infinity;
        let nearest_y = Infinity;

        for (let j = 0; j < NUMPARTICLES; j++) {
            if (i === j) continue;

            let other_x = particlesComputeData[2 * j];
            let other_y = particlesComputeData[2 * j + 1];

            let x = self_x - other_x;
            let x_squared = x * x;

            let y = self_y - other_y;
            let y_squared = y * y;

            // skip sqrt for better performance and less math
            // check again 9 since we are using squares
            let distance = x_squared + y_squared;
            if (distance < 9.0) continue;

            // get next closest neighbor
            if (distance < min_distance) {
                min_distance = distance;
                nearest_x = other_x;
                nearest_y = other_y;
            }

        }

        // if we've found a neighbor do movement logic and store new point values
        if (min_distance !== Infinity) {
            let return_value = move_towards(self_x, self_y, nearest_x, nearest_y);
            self_x = return_value[0];
            self_y = return_value[1];
        }

        particlesRenderData[2 * i] = self_x;
        particlesRenderData[2 * i + 1] = self_y;
    }
    
    // Send back new data back to main.js
    postMessage(null);
}