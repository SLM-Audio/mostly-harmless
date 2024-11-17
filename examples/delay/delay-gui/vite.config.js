import {defineConfig} from 'vite'
import react from '@vitejs/plugin-react'
import {resolve} from 'path'

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    build: {
        emptyOutDir: true,
        root: resolve('delay-gui'),
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
                gain_gui: resolve("delay-gui"),
                public: resolve("delay-gui/public"),
                src: resolve("delay-gui/src"),
                node_modules: resolve("delay-gui/node_modules")
            }
        }
    }
})
