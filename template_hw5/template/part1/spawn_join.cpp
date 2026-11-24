#include <vector>
#include <thread>
#include <utility>

// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.

// Loop from start to end for a current thread's chunk
void single_blur(const double *input, double *output, int size, int start, int end) {
    for (int i = start; i < end; i++) {
        // assume each thread might get an invalid first and last index
        // skip over them
        if (i == 0) {
            output[i] = input[i];
            continue;
        }
        if (i == size - 1) {
            output[i] = input[i];
            continue;
        }

        // calculate total sum of 3 variables and divide
        double sum = input[i - 1] + input[i] + input[i + 1];
        sum /= 3;

        output[i] = sum; // place into the output array
    }
}

void repeated_blur(double *input, double *output, int size, int repeats, int num_threads) {
    // Spawn and join threads repeatedly.
    int thread_chunk = (size + num_threads - 1) / num_threads;

    // init vector of threads of size num_threads
    std::vector<std::thread> worker_threads(num_threads);

    // Loop through repeat times
    for (int i = 0; i < repeats; i++) {
        // per each repeat create start and end for each thread id
        for (int id = 0; id < num_threads; id++) {

            // make start and end
            int start = id * thread_chunk;
            int end = start + thread_chunk;

            // if end is greater than size cap at size
            if (end > size) {
                end = size;
            }

            // launch threads
            worker_threads[id] = std::thread(single_blur, input, output, size, start, end);
        }

        // join threads
        for (int id = 0; id < num_threads; id++) {
            worker_threads[id].join();
        }

        // swap input output pointer
        std::swap(input, output);
    }
}