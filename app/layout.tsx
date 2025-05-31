
import type React from "react"
import type { Metadata } from "next"
import { Inter } from "next/font/google"
import Head from 'next/head'
import "./globals.css"
import { ThemeProvider } from "@/components/theme-provider"
import Navbar from "@/components/navbar"

const inter = Inter({ subsets: ["latin"] })

export const metadata: Metadata = {
  title: "Code Civilization - Daily Programming Challenges",
  description:
    "Join a community of developers who solve coding problems daily. Progress through tiers, unlock exclusive perks, and build your programming skills one challenge at a time.",
  icons: {
    icon: "/favicon.ico",
  },
    generator: 'v0.dev'
}

export default function RootLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <html>
        <Head>
          {/* Favicon link */}
          <link rel="icon" href="/logo.png" />
          {/* You can also add other favicon links or meta tags here */}
      </Head>
      <body className={inter.className}>
        <ThemeProvider attribute="class" defaultTheme="dark" enableSystem disableTransitionOnChange>
          <Navbar />
          <div className="pt-[72px]">{children}</div>
        </ThemeProvider>
      </body>
    </html>
  )
}
