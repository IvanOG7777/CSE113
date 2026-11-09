const updateSpriteWGSL = `
    
    struct Particle {
        pos : vec2<f32> 
    };

    struct Particles {
        particles : array<Particle>
    };

    // read only array of particles
    @binding(0) @group(0) var<storage, read_write> particlesA : Particles;
    // write only array of particles
    @binding(1) @group(0) var<storage, read_write> particlesB : Particles;
    
    
    // functon to move particles together
    // Practically the same as the other 2 implementations
    fn move_towards(self_pos: vec2<f32>, nearest_pos: vec2<f32>) -> vec2<f32> {
    
    var new_pos = self_pos;
    
    let step: f32 = 2.0 / 640.0;
    
    // Move X toward neighbor
    if (nearest_pos.x > self_pos.x) {
        new_pos.x += step;
    } else if (nearest_pos.x < self_pos.x) {
        new_pos.x -= step;
    }

    // Move Y toward neighbor
    if (nearest_pos.y > self_pos.y) {
        new_pos.y += step;
    } else if (nearest_pos.y < self_pos.y) {
        new_pos.y -= step;
    }

    return new_pos;
    
    }

    // 64 gpu invocations per workgroup, one particle per invocation
    @compute @workgroup_size(64)
    fn main(@builtin(global_invocation_id) GlobalInvocationID : vec3<u32>) {
        var index : u32 = GlobalInvocationID.x; // get threads global id from group
        
        // if index is out of bounds return
        if (index >= arrayLength(&particlesA.particles)) {
            return;
        }

        // Get position of current particle
        var vPos = particlesA.particles[index].pos;
        
        // init nearest neighbor tracking
        var min_distance: f32 = 1e30;
        var nearest_pos: vec2<f32>;
        
        let num_particles: u32 = arrayLength(&particlesA.particles);
        
        let radius = (3.0 * 2.0) / 640.0;
        let radius_squared = radius * radius;
        
        for (var j: u32 = 0u; j < num_particles; j++) {
        
            let other = particlesA.particles[j].pos;
            
            let x = vPos.x - other.x;
            let y = vPos.y - other.y;
            
            let x_squared = x * x;
            let y_squared = y * y;
            
            let distance = x_squared + y_squared;
            
            if (distance < radius_squared) {
                continue;
            }
            
            if (distance < min_distance) {
                min_distance = distance;
                nearest_pos.x = other.x;
                nearest_pos.y = other.y;
            }
        }
        
        if (min_distance != 1e30) {
        
            vPos = move_towards(vPos, nearest_pos);
        }


        // Write new particle data
        particlesB.particles[index].pos = vPos;
    }
`;

export default updateSpriteWGSL;