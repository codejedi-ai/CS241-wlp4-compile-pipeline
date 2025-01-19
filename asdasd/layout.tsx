import './globals.css'
import { Inter } from 'next/font/google'
import { SessionProvider } from "@/asdasd/components/session-provider"

const inter = Inter({ subsets: ['latin'] })

export const metadata = {
  title: 'Discord Auth App',
  description: 'Next.js app with Discord authentication',
}

export default function RootLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <html lang="en">
      <body className={inter.className}>
        <SessionProvider>{children}</SessionProvider>
      </body>
    </html>
  )
}

