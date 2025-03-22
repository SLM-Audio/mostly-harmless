import {defineConfig} from 'vite'
import react from '@vitejs/plugin-react'
import {resolve} from 'path'

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    build: {
        emptyOutDir: true,
        root: resolve('synth-gui'),
        rollupOptions: {
            output: {
                entryFileNames: '[name].js',
                chunkFileNames: '[name].js',
                assetFileNames: '[name].[ext]',
            }
        },
        resolve: {
            alias: {
                root: resolve("./"),
                synth_gui: resolve("synth-gui"),
                public: resolve("synth-gui/public"),
                src: resolve("synth-gui/src"),
                node_modules: resolve("synth-gui/node_modules")
            }
        }
    }
})
