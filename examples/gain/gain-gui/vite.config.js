import {defineConfig} from 'vite'
import react from '@vitejs/plugin-react'
import {resolve} from 'path'

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    build: {
        // outDir: resolve("../build/gain-gui"),
        emptyOutDir: true,
        root: resolve('gain-gui'),
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
                gain_gui: resolve("gain-gui"),
                public: resolve("gain-gui/public"),
                src: resolve("gain-gui/src"),
                node_modules: resolve("gain-gui/node_modules")
            }
        }
    }
})
