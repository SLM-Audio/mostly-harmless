import {defineConfig} from 'vite'
import react from '@vitejs/plugin-react'
import {resolve} from 'path'

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    build: {
        emptyOutDir: true,
        root: resolve('gain2-gui'),
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
                gain_gui: resolve("gain2-gui"),
                public: resolve("gain2-gui/public"),
                src: resolve("gain2-gui/src"),
                node_modules: resolve("gain2-gui/node_modules")
            }
        }
    }
})
